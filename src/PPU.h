//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_PPU_H
#define LINES_PPU_H

class Bus;


class PPU {
public:
    void connectBus(Bus& b) {
        this->bus = &b;
    }
private:
    Bus* bus = nullptr;
};


#endif //LINES_PPU_H