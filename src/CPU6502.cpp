//
// Created by Linus Bauer on 13.02.26.
//

#include "CPU6502.h"

#include <stdexcept>

#include "Bus.h"
#include <iostream>
#include <iomanip>
#include <string>

void CPU6502::connectBus(Bus& b) {
    this->bus = &b;
}

void CPU6502::connectNMIInterruptLine(InterruptLine& nmi) {
    nmiInterruptLine = &nmi;
}

void CPU6502::connectIRQ(InterruptLine& irq) {
    IRQ = &irq;
}

void CPU6502::startDMA(uint8_t) {

}

void CPU6502::reset() {
    pc = readAddress(0xFFFC);
    sp = 0xfd;
    sr = 0x34;
    a = x = y = 0;
}

void CPU6502::pollInterrupts() {
    if (nmiInterruptLine->consumeRisingEdge()) {
        nmiPending = true;
    }
    irqPending = IRQ->get();
}


uint8_t CPU6502::pop() {
    return bus->read(STACK_START + ++sp);
}

void CPU6502::push(const uint8_t value) {
    bus->write(STACK_START + sp--, value);
}

uint16_t CPU6502::popAddress() {
    const uint8_t low = bus->read(STACK_START + ++sp);
    const uint8_t high = bus->read(STACK_START + ++sp);
    return (high << 8) | low;
}

void CPU6502::pushAddress(const uint16_t value) {
    bus->write(STACK_START + sp--, (value >> 8) & 0xFF);
    bus->write(STACK_START + sp--, value & 0xFF);
}

uint16_t CPU6502::readAddress(const uint16_t addr) const {
    const uint16_t lo = bus->read(addr);
    const uint16_t hi = bus->read(addr + 1);
    return (hi << 8) | lo;
}

void CPU6502::branch(const uint8_t mask, const uint8_t predicate) {
    if ((sr & mask) == predicate) {
        if (current.pageCrossed) {
            current.cycle++;
        }
        current.cycle++;
        current.branchTaken = true;
    } else {
        current.branchTaken = false;
    }
}

void CPU6502::prepareBranch() {
    switch (current.instruction.operation) {
        case Operation::BCC:
            branch(CARRY, 0);
            break;
        case Operation::BCS:
            branch(CARRY, 1);
            break;
        case Operation::BEQ:
            branch(ZERO, 1);
            break;
        case Operation::BMI:
            branch(NEGATIVE, 1);
            break;
        case Operation::BNE:
            branch(ZERO, 0);
            break;
        case Operation::BPL:
            branch(NEGATIVE, 0);
            break;
        case Operation::BVC:
            branch(OVERFLOW, 0);
            break;
        case Operation::BVS:
            branch(OVERFLOW, 1);
            break;
        default:
            current.branchTaken = false;
    }
}

bool CPU6502::pageCrossed(const uint16_t base, const uint16_t addr) {
    return (base & 0xFF00) != (addr & 0xFF00);
}

void CPU6502::handleInterrupt(uint16_t vector, bool setBreakFlag) {
    push(pc >> 8 & 0xFF);
    push(pc & 0xFF);

    uint8_t pushed = sr | 0x20;
    if (setBreakFlag) {
        pushed |= BREAK;
    } else {
        pushed &= ~BREAK;
    }

    push(pushed);

    sr |= INTERRUPT_DISABLED;

    pc = readAddress(vector);
}


void CPU6502::handleBRK() {
    pc++;
    uint16_t vector = 0xFFFE;

    if (nmiPending) {
        vector = 0xFFFA;
        nmiPending = false;
    }

    handleInterrupt(vector, true);
}

void CPU6502::shiftL(uint8_t& m) {
    sr &= ~CARRY;
    sr |= (m & 0x80) ? CARRY : 0;
    m <<= 1;
    updateZN(m);
}

void CPU6502::shiftR(uint8_t& m) {
    sr &= ~CARRY;
    sr |= (m & 0x01) ? CARRY : 0;
    m >>= 1;
    updateZN(m);
}

void CPU6502::rotateLeft(uint8_t& m) {
    uint8_t rotated = m;
    const bool carry = sr & CARRY;
    sr &= ~CARRY;
    sr |= (rotated & 0x80) ? CARRY : 0;
    rotated = (rotated << 1) | (carry ? 1 : 0);
    updateZN(rotated);
    m = rotated;
}

void CPU6502::rotateRight(uint8_t& m) {
    uint8_t rotated = m;
    const bool carry = sr & CARRY;
    sr &= ~CARRY;
    sr |= (rotated & 0x01) ? CARRY : 0;
    rotated = (rotated >> 1) | (carry ? 0x80 : 0);
    updateZN(rotated);
    m = rotated;
}


void CPU6502::execute() {
    oddCycle ^= 1;
    pollInterrupts();

    if (current.cycle == 0) {
        if (nmiPending) {
            handleInterrupt(0xFFFA, false);
            nmiPending = false;
        } else if (irqPending && !(sr & INTERRUPT_DISABLED)) {
            handleInterrupt(0xFFFE, false);
            irqPending = false;
        }
        const uint8_t opcode = bus->read(pc++);
        current.instruction = instructions[opcode];
        current.pageCrossed = false;
        current.address = getAddress(current.instruction);
        current.cycle = current.instruction.cycles;
        prepareBranch();
    }
    printCPUState(pc, a, x, y, sp, sr, current);

    if (--current.cycle > 0) {
        return;
    }

    switch (current.instruction.operation) {
        case Operation::ADC: {
            const uint8_t operand = bus->read(current.address);
            const uint16_t sum = a + operand + ((sr & CARRY) == 1 ? 1 : 0);
            sr |= (sum & 0xFF00 ? CARRY : 0);
            sr |= (a ^ sum) & (operand ^ sum) & 0x80 ? CARRY : 0;
            updateZN(a);
            a = sum;
            break;
        }
        case Operation::AND:
            a &= bus->read(current.address);
            updateZN(0);
            break;
        case Operation::ASL:
            if (current.instruction.mode == AddressMode::ACC) {
                sr &= ~CARRY;
                sr |= (a & 0x80) ? CARRY : 0;
                a <<= 1;
                updateZN(a);
            } else {
                uint8_t m = bus->read(current.address);
                shiftL(m);
                bus->write(current.address, m);
            }
            break;

        // Branching
        case Operation::BCC:
        case Operation::BCS:
        case Operation::BEQ:
        case Operation::BMI:
        case Operation::BNE:
        case Operation::BPL:
        case Operation::BVC:
        case Operation::BVS:
            if (current.branchTaken) {
                pc = current.address;
                current.branchTaken = false;
            }
            break;

        case Operation::BIT: {
            const uint8_t operand = bus->read(current.address);
            sr &= ~(NEGATIVE | ZERO | CPU_OVERFLOW);
            sr |= !(operand & a) ? ZERO : 0;
            sr |= operand & (NEGATIVE | CPU_OVERFLOW);
            break;
        }
        case Operation::BRK:
            handleBRK();
            break;
        case Operation::CLC:
            setFlag(CARRY, false);
            break;
        case Operation::CLD:
            setFlag(DECIMAL, false);
            break;
        case Operation::CLI:
            setFlag(INTERRUPT_DISABLED, false);
            break;
        case Operation::CLV:
            sr &= ~CPU_OVERFLOW;
            break;
        case Operation::CMP: {
            const uint16_t diff = a - bus->read(current.address);
            sr &= ~CARRY;
            sr |= !(diff & 0x100) ? CARRY : 0;
            updateZN(diff);
            break;
        }
        case Operation::CPX: {
            const uint16_t diff = x - bus->read(current.address);
            sr &= ~CARRY;
            sr |= !(diff & 0x100) ? CARRY : 0;
            updateZN(diff);
            break;
        }
        case Operation::CPY: {
            const uint16_t diff = y - bus->read(current.address);
            sr &= ~CARRY;
            sr |= !(diff & 0x100) ? CARRY : 0;
            updateZN(diff);
            break;
        }
        case Operation::DEC:
            a--;
            updateZN(a);
            break;
        case Operation::DEX:
            x--;
            updateZN(x);
            break;
        case Operation::DEY:
            y--;
            updateZN(y);
            break;
        case Operation::EOR:
            a ^= bus->read(current.address);
            updateZN(a);
            break;
        case Operation::INC:
            a++;
            updateZN(a);
            break;
        case Operation::INX:
            x++;
            updateZN(x);
            break;
        case Operation::INY:
            y++;
            updateZN(y);
            break;
        case Operation::JMP:
            pc = current.address;
            break;
        case Operation::JSR: {
            push(pc - 1);
            pc = current.address;
            break;
        }
        // Register Load And Store
        case Operation::LDA:
            a = bus->read(current.address);
            updateZN(a);
            break;
        case Operation::LDX:
            x = bus->read(current.address);
            updateZN(x);
            break;
        case Operation::LDY:
            y = bus->read(current.address);
            updateZN(y);
            break;
        case Operation::STA:
            bus->write(current.address, a);
            break;
        case Operation::STX:
            bus->write(current.address, x);
            break;
        case Operation::STY:
            bus->write(current.address, y);
            break;

        case Operation::LSR:
            break;
        case Operation::NOP:
            break;
        case Operation::ORA:
            a |= bus->read(current.address);
            updateZN(a);
            break;


        case Operation::PHA:
            push(a);
            break;
        case Operation::PHP:
            push(sr | BREAK | UNUSED);
        case Operation::PLA:
            a = pop();
            updateZN(a);
            break;
        case Operation::PLP:
            sr = pop() | ~BREAK | ~UNUSED;
            break;
        case Operation::ROL: {
            if (current.instruction.mode == AddressMode::ACC) {
                rotateLeft(a);
            } else {
                uint8_t value = bus->read(current.address);
                rotateLeft(value);
                bus->write(current.address, value);
            }
            break;
        }
        case Operation::ROR: {
            if (current.instruction.mode == AddressMode::ACC) {
                rotateRight(a);
            } else {
                uint8_t value = bus->read(current.address);
                rotateRight(value);
                bus->write(current.address, value);
            }
            break;
        }
        case Operation::RTI: {
            sr &= (BREAK | UNUSED);
            sr |= pop() & ~(BREAK | UNUSED);
            const uint8_t low = pop();
            const uint8_t high = pop();
            pc = high << 8 | low;
            break;
        }
        case Operation::RTS: {
            pc = popAddress() + 1;
            break;
        }
        case Operation::SBC:
        case Operation::SEC:
            setFlag(CARRY, true);
            break;
        case Operation::SED:
            setFlag(DECIMAL, true);
            break;
        case Operation::SEI:
            setFlag(INTERRUPT_DISABLED, true);
            break;
        case Operation::TAX:
            x = a;
            updateZN(x);
            break;
        case Operation::TAY:
            y = a;
            updateZN(y);
            break;
        case Operation::TSX:
            x = sp;
            updateZN(x);
            break;
        case Operation::TXA:
            a = x;
            updateZN(a);
            break;
        case Operation::TXS:
            sp = x;
            break;
        case Operation::TYA:
            a = y;
            updateZN(a);
            break;

        case Operation::ALR:
        case Operation::ANC:
        case Operation::ANE:
        case Operation::ARR:
        case Operation::AXS:
        case Operation::LAX:
        case Operation::LAS:
        case Operation::SAX:
        case Operation::SHA:
        case Operation::SHS:
        case Operation::SHX:
        case Operation::SHY:
        case Operation::DCP:
        case Operation::ISB:
        case Operation::RLA:
        case Operation::RRA:
        case Operation::SLO:
        case Operation::SRE:
        case Operation::SKB:
        case Operation::IGN:
        case Operation::JAM:
        case Operation::TAS:
        case Operation::LXA:
        case Operation::SBX:
        case Operation::ISC:
        case Operation::USBC:
            break;
        default:
            throw std::runtime_error("Unknown opcode");
    }
}

uint16_t CPU6502::getAddress(const Instruction instruction) {
    switch (instruction.mode) {
        case AddressMode::NONE:
        case AddressMode::ACC:
        case AddressMode::IMPL:
            return 0;
        case AddressMode::IMT:
            return pc++;
        case AddressMode::ZPG: {
            const uint8_t operand = bus->read(pc++);
            return operand;
        }
        case AddressMode::ZPG_X: {
            const uint8_t operand = bus->read(pc++);
            const uint8_t addr = (operand + x) & 0xFF;
            return addr;
        }
        case AddressMode::ZPG_Y: {
            const uint8_t operand = bus->read(pc++);
            const uint8_t addr = (operand + y) & 0xFF;
            return addr;
        }
        case AddressMode::ABS: {
            const uint16_t addr = readAddress(pc);
            pc += 2;
            return addr;
        }
        case AddressMode::ABS_X: {
            const uint16_t base = readAddress(pc);
            pc += 2;
            const uint16_t addr = base + x;
            current.pageCrossed = pageCrossed(base, addr);
            switch (current.instruction.operation) {
                case Operation::STA:
                case Operation::ASL:
                case Operation::DEC:
                case Operation::INC:
                case Operation::LSR:
                case Operation::ROL:
                case Operation::ROR:
                case Operation::SLO:
                case Operation::RLA:
                case Operation::SRE:
                case Operation::RRA:
                case Operation::DCP:
                case Operation::ISB:
                case Operation::SHY:
                    break;
                default:
                    if (current.pageCrossed) {
                        current.cycle++;
                    }
                    break;
            }
            return addr;
        }
        case AddressMode::ABS_Y: {
            const uint16_t base = readAddress(pc);
            pc += 2;
            const uint16_t addr = base + y;
            current.pageCrossed = pageCrossed(base, addr);
            switch (current.instruction.operation) {
                case Operation::STA:
                case Operation::SLO:
                case Operation::RLA:
                case Operation::SRE:
                case Operation::RRA:
                case Operation::DCP:
                case Operation::ISB:
                case Operation::NOP:
                case Operation::SHX:
                case Operation::SHA:
                case Operation::SHS:
                    break;
                default:
                    if (current.pageCrossed) {
                        current.cycle++;
                    }
                    break;
            }
            return addr;
        }
        case AddressMode::REL: {
            const auto offset = static_cast<int8_t>(bus->read(pc++));
            const uint16_t addr = pc + offset;
            current.pageCrossed = pageCrossed(pc, addr);
            if (current.pageCrossed) {
                current.cycle++;
            }
            return addr;
        }
        case AddressMode::IDX_IND: {
            const uint8_t operand = bus->read(pc++);
            const uint8_t zp = (operand + x) & 0xFF;
            const uint16_t addr = bus->read(zp) | bus->read(zp + 1 & 0xFF) << 8;
            return addr;
        }
        case AddressMode::IND_IDX: {
            const uint8_t operand = bus->read(pc++);
            const uint16_t base = bus->read(operand) | bus->read(operand + 1 & 0xFF) << 8;
            const uint16_t addr = base + y;
            current.pageCrossed = pageCrossed(base, addr);
            switch (current.instruction.operation) {
                case Operation::STA:
                case Operation::SLO:
                case Operation::RLA:
                case Operation::SRE:
                case Operation::RRA:
                case Operation::DCP:
                case Operation::ISB:
                case Operation::NOP:
                case Operation::SHA:
                    break;
                default:
                    if (current.pageCrossed) {
                        current.cycle++;
                    }
                    break;
            }
            return addr;
        }
        case AddressMode::IND: {
            const uint16_t loByte = bus->read(pc++);
            const uint16_t hiByte = bus->read(pc++);
            const uint16_t pointer = loByte | hiByte << 8;
            uint8_t lo = bus->read(pointer);
            uint8_t hi;
            if ((pointer & 0xFF) == 0xFF) {
                hi = bus->read(pointer & 0xFF00);
            } else {
                hi = bus->read(pointer + 1);
            }
            const uint16_t addr = hi << 8 | lo;
            return addr;
        }
        default:
            throw std::runtime_error("Unknown address mode");
    }
}

// Helper to get string names from enums
std::string operationToString(const Operation op) {
    switch (op) {
        case Operation::ADC: return "ADC"; case Operation::AND: return "AND"; case Operation::ASL: return "ASL";
        case Operation::BCC: return "BCC"; case Operation::BCS: return "BCS"; case Operation::BEQ: return "BEQ";
        case Operation::BIT: return "BIT"; case Operation::BMI: return "BMI"; case Operation::BNE: return "BNE";
        case Operation::BPL: return "BPL"; case Operation::BRK: return "BRK"; case Operation::BVC: return "BVC";
        case Operation::BVS: return "BVS"; case Operation::CLC: return "CLC"; case Operation::CLD: return "CLD";
        case Operation::CLI: return "CLI"; case Operation::CLV: return "CLV"; case Operation::CMP: return "CMP";
        case Operation::CPX: return "CPX"; case Operation::CPY: return "CPY"; case Operation::DEC: return "DEC";
        case Operation::DEX: return "DEX"; case Operation::DEY: return "DEY"; case Operation::EOR: return "EOR";
        case Operation::INC: return "INC"; case Operation::INX: return "INX"; case Operation::INY: return "INY";
        case Operation::JMP: return "JMP"; case Operation::JSR: return "JSR"; case Operation::LDA: return "LDA";
        case Operation::LDX: return "LDX"; case Operation::LDY: return "LDY"; case Operation::LSR: return "LSR";
        case Operation::NOP: return "NOP"; case Operation::ORA: return "ORA"; case Operation::PHA: return "PHA";
        case Operation::PHP: return "PHP"; case Operation::PLA: return "PLA"; case Operation::PLP: return "PLP";
        case Operation::ROL: return "ROL"; case Operation::ROR: return "ROR"; case Operation::RTI: return "RTI";
        case Operation::RTS: return "RTS"; case Operation::SBC: return "SBC"; case Operation::SEC: return "SEC";
        case Operation::SED: return "SED"; case Operation::SEI: return "SEI"; case Operation::STA: return "STA";
        case Operation::STX: return "STX"; case Operation::STY: return "STY"; case Operation::TAX: return "TAX";
        case Operation::TAY: return "TAY"; case Operation::TSX: return "TSX"; case Operation::TXA: return "TXA";
        case Operation::TXS: return "TXS"; case Operation::TYA: return "TYA";
        default: return "UNK";
    }
}

std::string addressModeToString(AddressMode mode) {
    switch (mode) {
        case AddressMode::NONE: return "NONE"; case AddressMode::ACC: return "ACC"; case AddressMode::IMPL: return "IMPL";
        case AddressMode::IMT: return "IMM"; case AddressMode::ZPG: return "ZPG"; case AddressMode::ZPG_X: return "ZPG,X";
        case AddressMode::ZPG_Y: return "ZPG,Y"; case AddressMode::ABS: return "ABS"; case AddressMode::ABS_X: return "ABS,X";
        case AddressMode::ABS_Y: return "ABS,Y"; case AddressMode::REL: return "REL"; case AddressMode::IDX_IND: return "(IND,X)";
        case AddressMode::IND_IDX: return "(IND),Y"; case AddressMode::IND: return "IND";
        default: return "UNK";
    }
}

void printCPUState(uint16_t pc, uint8_t a, uint8_t x, uint8_t y, uint8_t sp, uint8_t sr, InstructionState instrState) {
    std::cout << "\033[3F";
    std::cout << "\033[J";

    std::cout << "CPU STATE\n";
    std::cout << "PC: $" << std::hex << std::setw(4) << std::setfill('0') << pc
              << "  A: $" << std::setw(2) << +a
              << "  X: $" << std::setw(2) << +x
              << "  Y: $" << std::setw(2) << +y
              << "  SP: $" << std::setw(2) << +sp
              << "  SR: $" << std::setw(2) << +sr << "\n";

    // Print instruction state
    std::cout << "INSTR: " << operationToString(instrState.instruction.operation)
              << "  MODE: " << addressModeToString(instrState.instruction.mode)
              << "  ADDR: $" << std::setw(4) << instrState.address
              << "  CYCLE: " << +instrState.cycle
              << "  BRANCH: " << (instrState.branchTaken ? "T" : "F")
              << "  PAGE: " << (instrState.pageCrossed ? "Y" : "N") << "\n";

    std::cout.flush();
}
