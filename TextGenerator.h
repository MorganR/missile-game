// A class for handling text generation

#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include <ft2build.h>
#include FT_FREETYPE_H

class TextGenerator {
    public:
    static FT_Library library;
    static FT_Face arial;
    static bool isInitialized;
    
    static void Initialize();
    static void PrintChar(int charCode);
};

#endif
