//
// Created by Linus Bauer on 14.02.26.
//

#include "Cartridge.h"

#include <fstream>
#include <iostream>
#include <vector>

Cartridge::Cartridge(const std::string& gamePath) {
    std::ifstream file(gamePath);
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
    const size_t prg_size = header.prg_rom_chunks * 16384;
    const size_t chr_size = header.chr_rom_chunks * 8192;
    if (header.mapper1 & 0x04) {
        std::cerr << "Not implemented" << std::endl;
    }
    file.close();
}

