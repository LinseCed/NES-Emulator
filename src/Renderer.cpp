//
// Created by Linus Bauer on 13.03.26.
//

#include "Renderer.h"

#include "glad/gl.h"

Renderer::Renderer() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}


void Renderer::render(std::array<uint32_t, 256 * 240>& frameBuffer) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 240, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.data());
}
