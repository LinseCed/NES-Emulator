//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_PPU_H
#define LINES_PPU_H
#include <array>
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
    bool execute();
    void writeCtrl(uint8_t c);
    void writeMask(uint8_t m);
    void writeOAMAddr(uint8_t a);
    [[nodiscard]] std::array<uint32_t, 256 * 240>* getFrameBuffer() const;
private:
    bool firstWrite = true;
    uint8_t vram[0x4000] = {};
    uint8_t status = 0;
    uint8_t ctrl = 0;
    uint8_t mask = 0;
    uint16_t addr = 0;
    uint16_t scanLine = 261;
    uint16_t cycle = 0;
    Bus* bus = nullptr;
    InterruptLine& nmiIRQ;
    uint16_t oamAddr = 0;
    std::array<uint32_t, 256 * 240>* frameBuffer{};
};


#endif //LINES_PPU_H