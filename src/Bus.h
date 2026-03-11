//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_BUS_H
#define LINES_BUS_H
#include <cstdint>

#include "Cartridge.h"
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

    void connectCartridge(Cartridge& c);
private:
    PPU& ppu;
    RAM& ram;
    CPU6502& cpu;
    Cartridge* cartridge = nullptr;
    bool nmiLine = false;
};


#endif //LINES_BUS_H