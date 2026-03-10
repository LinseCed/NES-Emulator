//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_EMULATOR_H
#define LINES_EMULATOR_H
#include "Bus.h"
#include "CPU6502.h"
#include "InterruptLine.h"
#include "PPU.h"
#include "RAM.h"


class Emulator {
public:
    Emulator();
private:
    InterruptLine nmiIRQ;
    InterruptLine apuIRQ;
    InterruptLine mapperIRQ;
    PPU ppu;
    RAM ram;
    CPU6502 cpu;
    Bus bus;
};


#endif //LINES_EMULATOR_H