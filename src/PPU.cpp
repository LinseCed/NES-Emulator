//
// Created by Linus Bauer on 13.02.26.
//

#include "PPU.h"

#include "InterruptLine.h"

uint8_t PPU::readStatus() {
    const uint8_t value = status;
    w = 1;
    status &= 0x7F;
    nmiIRQ.set(false);
    return value;
}

void PPU::writeCtrl(const uint8_t c) {
    ctrl = c;
}

void PPU::writeMask(const uint8_t m) {
    mask = m;
}

void PPU::writeOAMAddr(const uint8_t addr) {
    oamAddr = addr;
}

uint8_t PPU::readPPU() {
    return readVRAM(v);
}

void PPU::writePPU(uint8_t value) {
    writeVRAM(v, value);
}

uint8_t PPU::readOAMData() const {
    return oamData[oamAddr];
}

void PPU::writeOAMData(const uint8_t value) {
    oamData[oamAddr] = value;
    oamAddr++;
}

void PPU::writeAddr(uint8_t value) {
    if (w == 0) {
        t = (t & 0x00ff) |((value & 0x3f) << 8);
        w = 1;
    } else {
        t = (t & 0x7f00 | value);
        v = t;
        w = 0;
    }
}

void PPU::writeScroll(const uint8_t value) {
    if (w == 0) {
        x = value & 0x07;
        t = (t & 0x7fe0) | (value >> 3);
        w = 1;
    } else {
        t = (t & 0x0c1f) | ((value & 0x07) << 12) | ((value & 0xf8) << 2);
        w = 0;
    }
}


uint8_t PPU::readVRAM(uint16_t address) {
    address = address & 0x3fff;
    if (address < 0x2000) {

    }
    if (address < 0x3f00) {
        return vram[(address - 0x2000) % 0x800];
    }
    address = (address - 0x3f00 & 0x20);
    return palette[address];
}

void PPU::writeVRAM(uint16_t address, const uint8_t data) {
    address = address & 0x3fff;
    if (address < 0x2000) {

    } else if (address < 0x3f00) {
        vram[address] = data;
    } else {

    }
    v += ((ctrl & 0x4) ? 32 : 1);
}

bool PPU::execute() {
    bool frameComplete = false;
    cycle++;
    if (cycle > 340) {
        cycle = 0;
        scanLine++;
        if (scanLine > 261) {
            scanLine = 0;
        }
    }

    if (scanLine == 241 && cycle == 1) {
        status |= 0x80;
        frameComplete = true;
        if (ctrl & 0x80) {
            nmiIRQ.set(true);
        }
    }

    if (scanLine == 261 && cycle == 1) {
        status &= 0x7F;
        nmiIRQ.set(false);
    }

    return frameComplete;
}

const std::array<uint32_t, 256 * 240>* PPU::getFrameBuffer() const {
    return &frameBuffer;
}

