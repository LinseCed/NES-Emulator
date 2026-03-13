//
// Created by Linus Bauer on 13.03.26.
//

#ifndef LINES_WINDOW_H
#define LINES_WINDOW_H
#include "GLFW/glfw3.h"


class Window {
public:
    Window();
    [[nodiscard]] bool shouldClose() const;
private:
    GLFWwindow* window;
};


#endif //LINES_WINDOW_H