//
// Created by Linus Bauer on 13.02.26.
//

#include "Bus.h"

#include "RAM.h"


uint8_t Bus::read(uint16_t addr) const {
    if (addr <= 0x1fff) {
        return ram.read(addr);
    }
    return 0;
}

void::Bus::write(uint16_t addr, uint8_t data) const {
    if (addr <= 0x1FFF) {
        ram.write(addr & 0x07ff, data);
    } else if (addr <= 0x3fff) {

    } else if (addr <= 0x4020) {
        if (addr == 0x4014) {
            cpu.startDMA(data);
        }
    }
}
