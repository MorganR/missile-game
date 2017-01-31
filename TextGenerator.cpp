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

    error = FT_Set_Char_Size(
        arial,    /* handle to face object           */
        0,       /* char_width in 1/64th of points  */
        16*64,   /* char_height in 1/64th of points */
        0     /* horizontal device resolution    */
        0 );   /* vertical device resolution      */
    if ( error )
    {
        throw new std::runtime_error("Couldn't set font size.");
    }
}

void TextGenerator::PrintChar(int charCode)
{
    // Find the glyph index
    FT_UInt glyphIndex = FT_Get_Char_Index(arial, charCode);
    // Load the glyph from the index
    int error = FT_Load_Glyph(arial, glyphIndex, FT_LOAD_DEFAULT);
    // Rasterize the glyph if not in bitmap format
    if (arial->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL)
    }

    std::cout << "Char measurements\n\tbitmap_left: " << arial->glyph->bitmap_left << "\n\tbitmap_top: " << arial->glyph->bitmap_top << std::endl;
}

FT_Library TextGenerator::library;
FT_Face TextGenerator::arial;
bool TextGenerator::isInitialized = false;