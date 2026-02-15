//
// Created by Linus Bauer on 13.02.26.
//

#include "CPU6502.h"

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

void CPU6502::execute() {
    oddCycle ^= 1;
    if (instructionCycle == 0) {

    }

    uint8_t opcode = bus->read(pc++);
    Instruction instruction = instructions[opcode];
}

