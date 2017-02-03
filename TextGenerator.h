// A class for handling text generation

#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include "vector.h"

class TextGenerator {
    public:
    static void PrintString(const char * s, float x, float y, void * text_bitmap, vector const & colour);
};

#endif
