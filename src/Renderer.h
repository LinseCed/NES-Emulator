//
// Created by Linus Bauer on 13.03.26.
//

#ifndef LINES_RENDERER_H
#define LINES_RENDERER_H
#include <array>

#include <glad/gl.h>

class Renderer {
public:
    Renderer();
    void render(std::array<uint32_t, 256 * 240>& frameBuffer);
private:
    GLuint texture;
};


#endif //LINES_RENDERER_H