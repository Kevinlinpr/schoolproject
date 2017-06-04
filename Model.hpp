#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <iostream>

/* glm headers */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "skinned_mesh.hpp"

class Model
{
public:
    Model();
    ~Model();
    void render(GLuint shaderProgram);
    void calcAndSetModelMatrix(GLuint shaderProgram);
    void setBoneTransformations(GLuint shaderProgram, GLfloat currentTime);
    void setVisible(bool v) {visible = v;}
    void setAnimation(unsigned int a)
    {
        if(a >= 0 && a < numAnimations)
        {
            currentAnimation = a;
            mesh->setAnimation(a);
        }
    }
    bool loadMesh(std::string path);
private:
    SkinnedMesh *mesh;
    glm::vec3 translate;
    glm::vec3 rotate; // rotate in grades for each axis
    glm::vec3 scale;
    unsigned int currentAnimation;
    unsigned int numAnimations;
    bool visible;
};

#endif//MODEL_HEADER
