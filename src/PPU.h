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
    [[nodiscard]] const std::array<uint32_t, 256 * 240>* getFrameBuffer() const;
    void writePPU(uint8_t value);
    uint8_t readPPU();
    void writeOAMData(uint8_t value);
    [[nodiscard]] uint8_t readOAMData() const;
    void writeAddr(uint8_t value);
    void writeScroll(uint8_t value);
private:
    [[nodiscard]] uint8_t readVRAM(uint16_t address);
    void writeVRAM(uint16_t address, uint8_t data);
    bool firstWrite = true;
    uint8_t vram[0x0800] = {};
    uint8_t status = 0;
    uint8_t ctrl = 0;
    uint8_t mask = 0;
    uint16_t scanLine = 261;
    uint16_t cycle = 0;
    Bus* bus = nullptr;
    InterruptLine& nmiIRQ;
    uint16_t v;
    uint16_t t = 0;
    uint8_t x = 0;
    uint8_t w = 1;
    uint8_t oamAddr = 0;
    uint8_t oamData[256] = {};
    uint8_t palette[32] = {};
    std::array<uint32_t, 256 * 240> frameBuffer{};
};

static constexpr uint32_t nesPalette[64] = {
    0xff666666, 0xff002a88, 0xff1412a7, 0xff3b00a4, 0xff5c007e, 0xff6e0040, 0xff6c0600, 0xff561d00,
    0xff333500, 0xff0b4800, 0xff005200, 0xff004f08, 0xff00404d, 0xff000000, 0xff000000, 0xff000000,
    0xffadadad, 0xff155fd9, 0xff4240ff, 0xff7527fe, 0xffa01acc, 0xffb71e7b, 0xffb53120, 0xff994e00,
    0xff6b6d00, 0xff388700, 0xff0c9300, 0xff008f32, 0xff007c8d, 0xff000000, 0xff000000, 0xff000000,
    0xfffffeff, 0xff64b0ff, 0xff9290ff, 0xffc676ff, 0xfff36aff, 0xfffe6ecc, 0xfffe8170, 0xffea9e22,
    0xffbcbe00, 0xff88d800, 0xff5ce430, 0xff45e082, 0xff48cdde, 0xff4f4f4f, 0xff000000, 0xff000000,
    0xfffffeff, 0xffc0dfff, 0xffd3d2ff, 0xffe8c8ff, 0xfffbc2ff, 0xfffec4ea, 0xfffeccc5, 0xfff7d8a5,
    0xffe4e594, 0xffcfef96, 0xffbdf4ab, 0xffb3f3cc, 0xffb5ebf2, 0xffb8b8b8, 0xff000000, 0xff000000,
};

#endif //LINES_PPU_H