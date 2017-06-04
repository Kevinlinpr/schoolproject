#ifndef SHADERS
#define SHADERS

#include <SDL2/SDL.h>
#include <GL/glew.h>

GLuint compileShader( const char *file, GLenum shaderType ); // loads and compiles the shader and returns its ID
GLuint createProgram(); // creates a shader program and returns its ID
void attachShader( GLuint shader, GLuint program ); // attaches shader to a program
GLuint linkProgram( GLuint program ); // links compilled shader to the program
void deleteShader(GLuint shader); // deletes shader

#endif
