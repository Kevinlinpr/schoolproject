#include "shader.hpp"

#include "fileIO.hpp"

GLuint compileShader( const char *file, GLenum shaderType )
{ // loads and compiles the shader and returns its ID
  // returns 1=ok, 0=failed
  GLuint shader = 0;
  shader = glCreateShader( shaderType );
  if( shader == 0 )
    printf( "Shader %s could not be created\n", file );
  // load file with shader
  const GLchar *shaderSource = readFile( file, READ_TEXT );
  glShaderSource( shader, 1, &shaderSource, NULL );
  glCompileShader( shader );
  // check for shader compile errors
  GLint success = 0;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
  if( success == GL_FALSE )
    {
      GLint maxL = 0;
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxL );
      GLchar *errorLog = ( GLchar* )malloc( maxL * sizeof( GLchar ) );
      glGetShaderInfoLog( shader, maxL, &maxL, errorLog );
      glDeleteShader( shader );
      printf( "%s\n", errorLog );
      printf( "Shader %s could not be compiled\n", file );
      return 0;
    }
  return shader;
}

GLuint createProgram()
{ // creates a shader program and returns its ID
  return glCreateProgram();
}

void attachShader( GLuint shader, GLuint program )
{ // attaches shader to a program
  glAttachShader( program, shader );
  //glBindAttribLocation( program, 0, "vertexPosition" );
}

GLuint linkProgram( GLuint program )
{ // links compilled shader to the program
  // return 0 = failed, 1 = OK
  glLinkProgram( program );
  // check for errors
  GLint isLinked = 0;
  glGetProgramiv( program, GL_LINK_STATUS, &isLinked );
  if( isLinked == GL_FALSE )
    {
      GLint maxL = 0;
      glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxL );
      GLchar *errorLog = ( GLchar* )malloc( maxL * sizeof( GLchar ) );
      glGetProgramInfoLog( program, maxL, &maxL, errorLog );
      printf( "%s\n", errorLog );
      printf( "Error, shader program could not be linked\n" );
      return 0;
    }
  return 1;
}

void deleteShader(GLuint shader) // deletes shader
{
    glDeleteShader(shader);
}

