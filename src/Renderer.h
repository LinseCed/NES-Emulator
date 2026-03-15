//
// Created by Linus Bauer on 13.03.26.
//

#ifndef LINES_RENDERER_H
#define LINES_RENDERER_H
#include <array>
#include <string>

#include <glad/gl.h>

class Renderer {
public:
    Renderer();
    void render(const std::array<uint32_t, 256 * 240>& frameBuffer);
private:
    GLuint texture;
    GLuint program;
    GLuint VBO, VAO;
    const GLchar* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "    TexCoord = aTexCoord;\n"
    "}\n";
    const GLchar* fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D screenTexture;\n"
    "void main() {\n"
    "    FragColor = texture(screenTexture, TexCoord);\n"
    "}\n";
    std::array<uint32_t, 256*240> testFramebuffer;

    void fillTestFramebuffer() {
        for (int y = 0; y < 240; y++) {
            for (int x = 0; x < 256; x++) {
                uint8_t r = x;
                uint8_t g = y;
                uint8_t b = 255;
                uint8_t a = 255;
                testFramebuffer[y * 256 + x] = (b << 24) | (g << 16) | (r << 8) | a;
            }
        }
    }
};


#endif //LINES_RENDERER_H