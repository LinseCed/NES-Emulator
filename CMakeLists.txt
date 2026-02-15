cmake_minimum_required(VERSION 4.0)
project(Lines)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lines src/main.cpp
        src/Emulator.cpp
        src/Emulator.h
        src/CPU6502.cpp
        src/CPU6502.h
        src/PPU.cpp
        src/PPU.h
        src/RAM.cpp
        src/RAM.h
        src/Bus.cpp
        src/Bus.h
        src/Cartridge.cpp
        src/Cartridge.h
        src/InterruptLine.h
        src/Util/Util.h
)
