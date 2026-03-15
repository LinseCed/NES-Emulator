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
}

void Emulator::runFrame() {
    using clock = std::chrono::high_resolution_clock;
    constexpr auto frameTime = std::chrono::milliseconds(16);

    const auto frameStart = clock::now();

    int cyclesThisFrame = 0;
    constexpr int cyclesPerFrame = CYCLES_PER_SECOND / FRAMES_PER_SECOND;

    while (cyclesThisFrame < cyclesPerFrame) {
        cpu.execute();

        for (int i = 0; i < 3; i++) {
            ppu.execute();
        }

        cyclesThisFrame++;
    }

    const auto frameEnd = clock::now();
    if (const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        elapsed < frameTime) {
        std::this_thread::sleep_for(frameTime - elapsed);
        }
}

std::array<uint32_t, 256 * 240>* Emulator::getFrameBuffer() const {
    return ppu.getFrameBuffer();
}



