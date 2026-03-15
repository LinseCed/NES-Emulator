//
// Created by Linus Bauer on 13.02.26.
//

#include "PPU.h"

#include "InterruptLine.h"

uint8_t PPU::readStatus() {
    const uint8_t value = status;
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

void PPU::writeOAMAddr(const uint8_t a) {
    if (firstWrite) {
        firstWrite = false;
    } else {
        firstWrite = true;
    }
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

std::array<uint32_t, 256 * 240>* PPU::getFrameBuffer() const {
    return frameBuffer;
}

