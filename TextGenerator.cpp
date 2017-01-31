#include "TextGenerator.h"

#include <stdexcept>
#include <iostream>

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
                    &arial );
    if ( error )
    {
        throw new std::runtime_error("Couldn't load FreeSans.ttf font.");
    }

    isInitialized = true;

    std::cout << "Finished loading font." << std::endl;
    std::cout << "Num chars: " << arial->num_glyphs << std::endl;
    if (arial->face_flags & FT_FACE_FLAG_SCALABLE)
    {
        std::cout << "Font can be rendered in any size." << std::endl;
    }
}

FT_Library TextGenerator::library;
FT_Face TextGenerator::arial;
bool TextGenerator::isInitialized = false;