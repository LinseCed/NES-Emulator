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
#define CYCLES_PER_SECOND 1789773
#define FRAMES_PER_SECOND 60

class Emulator {
public:
    Emulator();
    void runFrame();
    std::array<uint32_t, 256 * 240> getFrameBuffer() const;
private:
    InterruptLine nmiIRQ;
    InterruptLine IRQ;
    InterruptLine mapperIRQ;
    PPU ppu;
    RAM ram;
    CPU6502 cpu;
    Bus bus;
};


#endif //LINES_EMULATOR_H