// A class for handling text generation

#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <string>

#include "gpuProgram.h"

class TextGenerator {
    public:
    static FT_Library library;
    static FT_Face face;
    static bool isInitialized;
    static GPUProgram * gpuProgram;

    static void Initialize();
    static void PrintChar(int charCode);
    static void GetCharTexture(int charCode, GLuint * texture);
    static void RenderText(GLuint texture, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif
