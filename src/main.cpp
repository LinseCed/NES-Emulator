#include "Emulator.h"
#include "Renderer.h"
#include "Window.h"

int main() {
    Emulator emulator;
    const Window window;
    Renderer renderer;
    while (!window.shouldClose()) {
        emulator.runFrame();

        renderer.render(emulator.getFrameBuffer());

        window.swapBuffers();
        Window::pollEvents();
    }
}
