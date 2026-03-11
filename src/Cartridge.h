//
// Created by Linus Bauer on 14.02.26.
//

#ifndef LINES_CARTRIDGE_H
#define LINES_CARTRIDGE_H
#include <string>
#include <vector>

struct INESHeader {
    char name[4];
    uint8_t prg_rom_chunks;
    uint8_t chr_rom_chunks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prg_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char unused[5];
};


class Cartridge {
public:
    explicit Cartridge(const std::string& gamePath);
    std::vector<uint8_t> prgROM;
    std::vector<uint8_t> chrROM;
};


#endif //LINES_CARTRIDGE_H