//
// Created by Linus Bauer on 13.02.26.
//

#include "Emulator.h"

#include <thread>

#include "Cartridge.h"

Emulator::Emulator() : ppu(nmiIRQ), bus(ppu, ram, cpu) {
    ppu.connectBus(bus);
    cpu.connectBus(bus);
    cpu.connectNMIInterruptLine(nmiIRQ);
    cpu.connectIRQ(IRQ);
    Cartridge cartridge("SMB.nes");
    bus.connectCartridge(cartridge);


    using clock = std::chrono::high_resolution_clock;
    constexpr auto frameTime = std::chrono::milliseconds(16);
    while (true) {
        auto frameStart = clock::now();
        int cyclesThisFrame = 0;
        int cyclesPerFrame = CYCLES_PER_SECOND / FRAMES_PER_SECOND;
        while (cyclesThisFrame < cyclesPerFrame) {
            cpu.execute();
            ppu.execute();
            ppu.execute();
            ppu.execute();
            cyclesThisFrame++;
        }

        auto frameEnd = clock::now();
        if (auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
            elapsed < frameTime) {
            std::this_thread::sleep_for(frameTime - elapsed);
        }
    }
}
