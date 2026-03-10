//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_BUS_H
#define LINES_BUS_H
#include <cstdint>

#include "CPU6502.h"

class PPU;
class RAM;
class CPU6502;

class Bus {
public:
    Bus(PPU &ppu, RAM& ram, CPU6502& cpu) : ppu(ppu), ram(ram), cpu(cpu) {};
    [[nodiscard]] uint8_t read(uint16_t addr) const;
    void write(uint16_t addr, uint8_t data) const;

    void setNMI(bool state) {
        nmiLine = state;
    }

    [[nodiscard]] bool getNMI() const {
        return nmiLine;
    }
private:
    PPU& ppu;
    RAM& ram;
    CPU6502& cpu;
    bool nmiLine = false;
};


#endif //LINES_BUS_H