#include "Grid.hpp"
#include <iostream>
#include "texture.hpp"

void Grid::render(GLuint shaderProgram)
{
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, imageID );
    
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    
    GLint textureLocation = glGetUniformLocation( shaderProgram, "sprite" );
    glUniform1i( textureLocation, 0 );
    
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
    
    glDisableVertexAttribArray( 0 );
    glDisableVertexAttribArray( 1 );
    glDisableVertexAttribArray( 2 );
      
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    glBindTexture( GL_TEXTURE_2D, 0 );
}

Grid::Grid()
{
    GLfloat data[] = { -100.0f,  0.0f, -100.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
                        100.0f,  0.0f, -100.0f,  0.0f, 1.0f, 0.0f,   100.0f, 0.0f,
                       -100.0f,  0.0f,  100.0f,  0.0f, 0.0f, 1.0f,   0.0f, 100.0f,
                        100.0f,  0.0f,  100.0f,  0.0f, 1.0f, 0.0f,   100.0f, 100.0f   };

GLuint indices[] = { 0, 1, 2, // first triangle
                     1, 2, 3 // second triangle
                   };
    /* << Set VAO & VBO */
  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );

  /* allocate memory for the buffer */
  glBufferData( GL_ARRAY_BUFFER, sizeof( data ), data, GL_STATIC_DRAW );
  
  /* set attrib for vertex position */
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8*sizeof( GLfloat ), (GLvoid*)0 );
  /* set attrib for vertex color */
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8*sizeof( GLfloat ), (GLvoid*)(3*sizeof( GLfloat )) ); 
  /* set sttrib for vertex uv coordonates */
  glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8*sizeof( GLfloat ), (GLvoid*)(6*sizeof( GLfloat )) ); 

  glGenBuffers( 1, &EBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );

  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_DYNAMIC_DRAW );
  
  
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  /* Unbind EBO. Must be done after unbinde the VAO because VAO to be able to bind EBO automaticly */
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  /* >> End set VAO & VBO */
  
  imageID = TextureFromFile("Textures/Grid.png");
}
