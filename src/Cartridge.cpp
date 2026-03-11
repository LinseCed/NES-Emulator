//
// Created by Linus Bauer on 14.02.26.
//

#include "Cartridge.h"

#include <fstream>
#include <iostream>
#include <vector>

Cartridge::Cartridge(const std::string& gamePath) {
    std::ifstream file(gamePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << gamePath << std::endl;
    }
    INESHeader header{};
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(file), {});
    if (data.at(0) != 0x4e || data.at(1) != 0x45 || data.at(2) != 0x53 || data.at(3) != 0x1a) {
        std::cerr << "Invalid file format" << std::endl;
    }

    if (header.mapper1 & 0x04) {
        std::cerr << "Not implemented" << std::endl;
    }

    const long prg_size = header.prg_rom_chunks * 16384;
    const long chr_size = header.chr_rom_chunks * 8192;

    long offset = sizeof(header);
    prgROM.assign(data.begin() + offset, data.begin() + offset + prg_size);
    offset += prg_size;
    if (chr_size > 0) {
        chrROM.assign(data.begin() + offset, data.begin() + offset + chr_size);
    }

    file.close();
}

