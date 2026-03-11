//
// Created by Linus Bauer on 13.02.26.
//

#include "CPU6502.h"

#include <stdexcept>

#include "Bus.h"

void CPU6502::connectBus(Bus& b) {
    this->bus = &b;
}

void CPU6502::connectNMIInterruptLine(InterruptLine& nmi) {
    nmiIRQ = &nmi;
}

void CPU6502::connectAPUInterruptLine(InterruptLine& apu) {
    apuIRQ = &apu;
}


void CPU6502::connectMapperInterruptLine(InterruptLine& mapper) {
    mapperIRQ = &mapper;
}

void CPU6502::startDMA(uint8_t) {

}

void CPU6502::reset() {
    pc = readAddress(0xfffc);
    sp = 0xfd;
    sr = 0x34;
    a = x = y = 0;
}

void CPU6502::pollInterrupts() {

}


uint8_t CPU6502::pop() {
    return bus->read(STACK_START + sp++);
}

void CPU6502::push(uint8_t value) {
    bus->write(STACK_START + sp--, value);
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

void CPU6502::execute() {
    oddCycle ^= 1;
    if (current.cycle == 0) {
        const uint8_t opcode = bus->read(pc++);
        current.instruction = instructions[opcode];
        current.pageCrossed = false;
        current.address = getAddress(current.instruction);
        current.cycle = current.instruction.cycles;
        prepareBranch();
    }

    if (--current.cycle > 0) {
        return;
    }

    switch (current.instruction.operation) {
        case Operation::ADC:
        case Operation::AND:
        case Operation::ASL:
        case Operation::BCC:
            if (current.branchTaken) {
                pc = current.address;
                current.branchTaken = false;
            }
            break;
        case Operation::BCS:
        case Operation::BEQ:
        case Operation::BIT:
        case Operation::BMI:
        case Operation::BNE:
        case Operation::BPL:
        case Operation::BRK:
        case Operation::BVC:
        case Operation::BVS:
        case Operation::CLC:
        case Operation::CLD:
        case Operation::CLI:
        case Operation::CLV:
        case Operation::CMP:
        case Operation::CPX:
        case Operation::CPY:
        case Operation::DEC:
        case Operation::DEX:
        case Operation::DEY:
        case Operation::EOR:
        case Operation::INC:
        case Operation::INX:
        case Operation::INY:
        case Operation::JMP:
        case Operation::JSR:
        case Operation::LDA:
            a = bus->read(current.address);
            updateZN(a);
            break;
        case Operation::LDX:
        case Operation::LDY:
        case Operation::LSR:
        case Operation::NOP:
        case Operation::ORA:
        case Operation::PHA:
        case Operation::PHP:
        case Operation::PLA:
        case Operation::PLP:
        case Operation::ROL:
        case Operation::ROR:
        case Operation::RTI:
        case Operation::RTS:
        case Operation::SBC:
        case Operation::SEC:
        case Operation::SED:
        case Operation::SEI:
        case Operation::STA:
        case Operation::STX:
        case Operation::STY:
        case Operation::TAX:
        case Operation::TAY:
        case Operation::TSX:
        case Operation::TXA:
        case Operation::TXS:
        case Operation::TYA:
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
            if (current.pageCrossed) {
                current.cycle++;
            }
            return addr;
        }
        case AddressMode::ABS_Y: {
            const uint16_t base = readAddress(pc);
            pc += 2;
            const uint16_t addr = base + y;
            current.pageCrossed = pageCrossed(base, addr);
            if (current.pageCrossed) {
                current.cycle++;
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
            if (current.pageCrossed) {
                current.cycle++;
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
