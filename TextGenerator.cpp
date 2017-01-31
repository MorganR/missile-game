#include "TextGenerator.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "headers.h"

void TextGenerator::Initialize()
{
    if (isInitialized)
        return;
    
    int error = FT_Init_FreeType( &library );
    if ( error )
    {
        throw new std::runtime_error("Couldn't initialize FreeType.");
    }

    error = FT_New_Face( library,
                    "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
                    0,
                    &face );
    if ( error )
    {
        throw new std::runtime_error("Couldn't load FreeSans.ttf font.");
    }

    error = FT_Set_Char_Size(
        face,    /* handle to face object           */
        0,       /* char_width in 1/64th of points  */
        16*64,   /* char_height in 1/64th of points */
        0,     /* horizontal device resolution    */
        0 );   /* vertical device resolution      */
    if ( error )
    {
        throw new std::runtime_error("Couldn't set font size.");
    }

    gpuProgram = new GPUProgram("textshader.vert", "textshader.frag");

    isInitialized = true;
}

void TextGenerator::PrintChar(int charCode)
{
    gpuProgram->activate();

    GLuint texture;
    GetCharTexture(charCode, &texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glm::mat4 projection = glm::ortho(0.0f, 600.0f, 0.0f, 400.0f);

    RenderText(texture, 0, 0, 1, glm::vec3(0.0, 1.0, 0.0));

    gpuProgram->deactivate();
}

void TextGenerator::GetCharTexture(int charCode, GLuint * texture)
{
    // load glyph image into the slot (erase previous one)
    int error = FT_Load_Char(face, charCode, FT_LOAD_RENDER);
    if ( error )
    {
        throw new std::runtime_error("Couldn't load char.");
    }

    std::cout << "Char measurements\n\tbitmap_left: " << face->glyph->bitmap_left << "\n\tbitmap_top: " << face->glyph->bitmap_top << std::endl;

    // Remove requirement for 4-byte aligned textures
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Generate texture
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextGenerator::RenderText(GLuint texture, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // Activate corresponding render state	
    glUniform3f(glGetUniformLocation(gpuProgram->program_id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLfloat xpos = x + face->glyph->bitmap_left * scale;
    GLfloat ypos = y - (face->glyph->bitmap.rows - face->glyph->bitmap_top) * scale;

    GLfloat w = face->glyph->bitmap.width * scale;
    GLfloat h = face->glyph->bitmap.rows * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, texture);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (face->glyph->advance.x >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FT_Library TextGenerator::library;
FT_Face TextGenerator::face;
bool TextGenerator::isInitialized = false;
GPUProgram * TextGenerator::gpuProgram;