//
// Created by Linus Bauer on 13.02.26.
//

#include "Emulator.h"

#include "Cartridge.h"

Emulator::Emulator() : bus(ppu, ram) {
    ppu.connectBus(bus);
    cpu.connectBus(bus);
    cpu.connectNMIInterruptLine(nmiIRQ);
    cpu.connectAPUInterruptLine(apuIRQ);
    cpu.connectMapperInterruptLine(mapperIRQ);
    Cartridge cartridge("SMB.nes");
}
