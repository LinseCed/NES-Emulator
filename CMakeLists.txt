cmake_minimum_required(VERSION 4.0)
project(Lines)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(external/glfw-3.4)

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
        src/Renderer.cpp
        src/Renderer.h
        src/Window.cpp
        src/Window.h
        external/glad/include/KHR/khrplatform.h
        external/glad/include/glad/gl.h
        external/glad/src/gl.c
)

target_include_directories(Lines PRIVATE external/glad/include)
target_include_directories(Lines PRIVATE external/glfw-3.4/include)
target_link_libraries(Lines glfw)