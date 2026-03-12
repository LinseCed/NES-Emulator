//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_PPU_H
#define LINES_PPU_H
#include <cstdint>

class InterruptLine;
class Bus;


class PPU {
public:
    explicit PPU(InterruptLine& nmiIRQ) : nmiIRQ(nmiIRQ) {};
    void connectBus(Bus& b) {
        this->bus = &b;
    }
    uint8_t readStatus();
    void execute();
private:
    uint8_t vram[0x4000] = {};
    uint8_t status = 0;
    uint8_t ctrl = 0;
    uint8_t mask = 0;
    uint16_t addr = 0;
    uint16_t scanLine = 261;
    uint16_t cycle = 0;
    Bus* bus = nullptr;
    InterruptLine& nmiIRQ;
};


#endif //LINES_PPU_H