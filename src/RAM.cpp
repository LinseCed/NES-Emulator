//
// Created by Linus Bauer on 13.02.26.
//

#include "RAM.h"

#include <cassert>

uint8_t RAM::read(const uint16_t address) const {
    assert(address <= 0x07FF);
    return ram.at(address);
}

void RAM::write(const uint16_t address, const uint8_t value) {
    assert(address <= 0x07FF);
    ram.at(address) = value;
}

