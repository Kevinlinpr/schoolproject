#ifndef GRID
#define GRID

#include <GL/glew.h>

class Grid
{
public:
    Grid();
    void render(GLuint shaderProgram);
private:
    GLuint VAO, VBO, EBO;
    GLuint imageID;
};

#endif//GRID
