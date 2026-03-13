//
// Created by Linus Bauer on 13.02.26.
//

#include "Bus.h"

#include <iostream>

#include "PPU.h"
#include "RAM.h"


uint8_t Bus::read(const uint16_t addr) const {
    if (addr <= 0x1fff) {
        return ram.read(addr & 0x07ff);
    }
    if (addr >= 0x2000 && addr <= 0x3fff) {
        if (addr == 0x2002) {
            return ppu.readStatus();
        }
        if (addr == 0x2004) {

        }
    }

    if (addr >= 0x4000 && addr <= 0x4017) {

    }

    if (addr >= 0x4020 && addr <= 0x5fff) {

    }

    if (addr >= 0x6000 && addr <= 0x7fff) {

    }

    if (addr >= 0x8000) {
        return cartridge->prgROM[addr - 0x8000];
    }
    return 0;
}

void::Bus::write(const uint16_t addr, const uint8_t data) const {
    if (addr <= 0x1FFF) {
        ram.write(addr & 0x07ff, data);
    } else if (addr <= 0x3fff) {

    } else if (addr >= 0x2000 && addr <= 0x2007) {
        if (addr == 0x2000) {
            ppu.writeCtrl(data);
        }
        if (addr == 0x2001) {
            ppu.writeMask(data);
        }
        if (addr == 0x2003) {

        }
        if (addr == 0x2004) {

        }
        if (addr == 0x2005) {

        }
        if (addr == 0x2006) {

        }
        if (addr == 0x2007) {

        }
    } else if (addr <= 0x4020) {
        if (addr == 0x4014) {
            cpu.startDMA(data);
        }
    }
}

void Bus::connectCartridge(Cartridge &c) {
    cartridge = &c;
}

