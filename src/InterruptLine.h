//
// Created by Linus Bauer on 15.02.26.
//

#ifndef LINES_INTERRUPTLINE_H
#define LINES_INTERRUPTLINE_H

class InterruptLine {
public:
    void set(const bool level) {
        if (!state && level) {
            risingEdge = true;
        }
        state = level;
    }

    [[nodiscard]] bool get() const {
        return state;
    }
    [[nodiscard]] bool consumeRisingEdge() {
        if (risingEdge) {
            risingEdge = false;
            return true;
        }
        return false;
    }
private:
    bool state = false;
    bool risingEdge = false;
};

#endif //LINES_INTERRUPTLINE_H