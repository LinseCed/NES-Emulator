//
// Created by Linus Bauer on 13.03.26.
//

#ifndef LINES_WINDOW_H
#define LINES_WINDOW_H
#include <glad/gl.h>
#include "GLFW/glfw3.h"


class Window {
public:
    Window();
    ~Window();
    [[nodiscard]] bool shouldClose() const;
    void swapBuffers() const;
    static void pollEvents() ;
private:
    GLFWwindow* window;
};


#endif //LINES_WINDOW_H