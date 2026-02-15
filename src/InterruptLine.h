//
// Created by Linus Bauer on 15.02.26.
//

#ifndef LINES_INTERRUPTLINE_H
#define LINES_INTERRUPTLINE_H

class InterruptLine {
public:
    void set(bool level) {
        state = level;
    }

    [[nodiscard]] bool get() const {
        return state;
    }
private:
    bool state = false;
};

#endif //LINES_INTERRUPTLINE_H