// GPUProgram class

#ifndef SHADER_H
#define SHADER_H


#include "headers.h"
#include <iostream>

class GPUProgram {
 public:

  unsigned int program_id;
  unsigned int shader_vp;
  unsigned int shader_fp;

  GPUProgram() {};

  GPUProgram( const char *vsFile, const char *fsFile ) {
    initFromFile( vsFile, fsFile );
  }

  void initFromFile( const char *vsFile, const char *fsFile ) {
    
    char* vsText = textFileRead(vsFile);	
    
    if (vsText == NULL) {
      std::cerr << "Vertex shader file '" << vsFile << "' not found." << std::endl;
      return;
    }
    
    char* fsText = textFileRead(fsFile);
    
    if (fsText == NULL) {
      std::cerr << "Fragment shader file '" << fsFile << "' not found." << std::endl;
      return;
    }
    
    std::cout << "Initializing GPUProgram." << std::endl;
    
    init( vsText, fsText );
  }

  ~GPUProgram() {
    glDetachShader( program_id, shader_vp );
    glDeleteShader( shader_vp );

    glDetachShader( program_id, shader_fp );
    glDeleteShader( shader_fp );

    glDeleteProgram( program_id );
  }

  void init( char *vsText, char *fsText );

  int id() {
    return program_id;
  }

  void activate() {
    glUseProgram( program_id );
  }

  void deactivate() {
    glUseProgram( 0 );
  }

  char* textFileRead(const char *fileName);


  void glErrorReport( char *where ) {

    GLuint errnum;
    const char *errstr;

    while ((errnum = glGetError())) {
      errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
      std::cerr << where << ": " << errstr << std::endl;
    }
  }

};

#endif