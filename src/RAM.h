//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_RAM_H
#define LINES_RAM_H
#include <array>
#include <cstdint>

#define RESET_VECTOR 0xfffc;

class RAM {
public:
    void write(uint16_t address, uint8_t value);
    [[nodiscard]] uint8_t read(uint16_t address) const;
private:
    std::array<uint8_t, 2048> ram{};
};


#endif //LINES_RAM_H