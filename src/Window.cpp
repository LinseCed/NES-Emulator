//
// Created by Linus Bauer on 13.03.26.
//

#include "Window.h"

#include <stdexcept>
#include <GLFW/glfw3.h>

Window::Window() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW3");
    }

    glfwDefaultWindowHints();

    window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);

    if (!window) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwShowWindow(window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}
