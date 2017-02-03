#include "TextGenerator.h"

#include "headers.h"

void TextGenerator::PrintString(const char * s, float x, float y, void * text_bitmap)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    int len = strlen(s);

    glRasterPos2f( x, y );

    for (int i=0; i<len; i++)
        glutBitmapCharacter( text_bitmap, (GLubyte) s[i] );

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
