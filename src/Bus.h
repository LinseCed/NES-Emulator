//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_BUS_H
#define LINES_BUS_H
#include <cstdint>

class PPU;
class RAM;


class Bus {
public:
    Bus(PPU &ppu, RAM& ram) : ppu(ppu), ram(ram) {};
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
    bool nmiLine = false;
};


#endif //LINES_BUS_H