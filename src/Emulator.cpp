//
// Created by Linus Bauer on 13.02.26.
//

#include "Emulator.h"

#include <iomanip>
#include <iostream>

#include "Cartridge.h"

Emulator::Emulator() : bus(ppu, ram, cpu) {
    ppu.connectBus(bus);
    cpu.connectBus(bus);
    cpu.connectNMIInterruptLine(nmiIRQ);
    cpu.connectAPUInterruptLine(apuIRQ);
    cpu.connectMapperInterruptLine(mapperIRQ);
    Cartridge cartridge("SMB.nes");
    bus.connectCartridge(cartridge);
    uint16_t counter = 0x8000;
    for (const uint8_t byte : cartridge.prgROM) {
        std::cout << "0x"
                  << std::hex               // switch to hex
                  << std::setw(2)          // width of 2 characters
                  << std::setfill('0')     // pad with 0 if needed
                  << static_cast<int>(bus.read(counter++))
                  << " ";
    }
    std::cout << std::dec << std::endl; // switch back to decimal
    cpu.execute();
}
