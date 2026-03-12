//
// Created by Linus Bauer on 13.02.26.
//

#include "PPU.h"

#include <assert.h>
#include <iostream>

#include "InterruptLine.h"

uint8_t PPU::readStatus() {
    const uint8_t value = status;
    status &= 0x7F;
    nmiIRQ.set(false);
    return value;
}

void PPU::execute() {
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
        if (ctrl & 0x80) {
            nmiIRQ.set(true);
        }
    }

    if (scanLine == 261 && cycle == 1) {
        status &= 0x7F;
        nmiIRQ.set(false);
    }
}

