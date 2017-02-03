// GPUProgram class


#include "gpuProgram.h"


char* GPUProgram::textFileRead(const char *fileName)

{
  std::cout << "Reading file: " << fileName << std::endl;
  char* text = "";
    
  if (fileName != NULL) {
    
    std::cout << "Opening file." << std::endl;
    FILE *file = fopen( fileName, "rt" );

    if (file == NULL) {

      std::cerr << "File " << fileName << " could not be opened" << std::endl;
      text = "";

    } else {
      std::cout << "Seeking within file." << std::endl;
      fseek(file, 0, SEEK_END);
      int count = ftell(file);
      rewind(file);
      
      std::cout << "Reviewing file data." << std::endl;
      if (count > 0) {
	text = (char*)malloc(sizeof(char) * (count + 1));
	count = fread(text, sizeof(char), count, file);
	text[count] = '\0';
      }

      std::cout << "Closing file." << std::endl;
      fclose(file);
    }
  }
  
  std::cout << "Returning." << std::endl;
  return _strdup(text);
}


static void validateShader(GLuint shader, const char* file = 0)

{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
    
  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

  if (length > 0)
    std::cout << "Shader " << shader << " (" << (file?file:"") << ") compile log: " << std::endl << buffer << std::endl;
}


static void validateProgram(GLuint program)

{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
    
  memset(buffer, 0, BUFFER_SIZE);

  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

  if (length > 0)
    std::cout << "Program " << program << " link log: " << buffer << std::endl;
    
  glValidateProgram(program);

  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

  if (status == GL_FALSE)
    std::cout << "Error validating program " << program << std::endl;
}


void GPUProgram::init( char *vsText, char *fsText )

{
  glErrorReport( "before GPUProgram::init" );

  std::cout << "Entered GPUProgram init." << std::endl;

  // Vertex shader

  shader_vp = glCreateShader(GL_VERTEX_SHADER);

  std::cout << "Modifying newly created shader." << std::endl;
  std::cout << "Seriously? did it fail?" << std::endl;
  std::cout << "Let's just keep trying here..." << std::endl;

  glShaderSource( shader_vp, 1, (const char**) &vsText, 0 );
  glCompileShader( shader_vp );
  validateShader( shader_vp, "vertex shader" );
    
  // Fragment shader

  std::cout << "Creating fragment shader." << std::endl;

  shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource( shader_fp, 1, (const char **) &fsText, 0 );
  glCompileShader( shader_fp );
  validateShader( shader_fp, "fragment shader" );
    
  // GLSL program

  std::cout << "Creating program." << std::endl;

  program_id = glCreateProgram();
  glAttachShader( program_id, shader_vp );
  glAttachShader( program_id, shader_fp );
  glLinkProgram( program_id );
  validateProgram( program_id );

  glErrorReport( "after GPUProgram::init" );
}
