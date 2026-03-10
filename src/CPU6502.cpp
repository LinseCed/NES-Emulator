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

uint16_t CPU6502::readAddress(uint16_t addr) const {
    const uint16_t lo = bus->read(addr);
    const uint16_t hi = bus->read(addr + 1);
    return (hi << 8) | lo;
}

void CPU6502::branch(uint8_t mask, uint8_t predicate) {
    if ((sr & mask) == predicate) {

    }
}

void CPU6502::prepareBranch() {

}


void CPU6502::execute() {
    oddCycle ^= 1;
    if (instructionCycle == 0) {
        const uint8_t opcode = bus->read(pc++);
        currentInstruction = instructions[opcode];
        currentAddress = getAddress(currentInstruction);
        prepareBranch();
    }

    if (instructionCycle == 1) {
        instructionCycle--;
    } else {
        instructionCycle--;
        return;
    }

    switch (currentInstruction.operation) {
        case Operation::ADC:
        case Operation::AND:
        case Operation::ASL:
        case Operation::BCC:
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
            a = readAddress(currentAddress);
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

uint16_t CPU6502::getAddress(Instruction instruction) const {
    AddressMode mode = instruction.mode;
    switch (mode) {
        case AddressMode::NONE:
            return 0;
        case AddressMode::ACC:
        case AddressMode::IMPL:
        case AddressMode::IMT:
        case AddressMode::ZPG:
        case AddressMode::ZPG_X:
        case AddressMode::ZPG_Y:
        case AddressMode::ABS:
        case AddressMode::ABS_X:
        case AddressMode::ABS_Y:
        case AddressMode::REL:
        case AddressMode::IDX_IND:
        case AddressMode::IND_IDX:
        default:
            throw std::runtime_error("Unknown address mode");
    }
}
