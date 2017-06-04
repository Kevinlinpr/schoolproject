#ifndef CAMERA
#define CAMERA

/* glm headers */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

class Camera
{
public:
#define PI 3.14159265359
    Camera();
    void calcCameraMatrices(GLuint mainWinW, GLuint mainWinH);
    void setMatrices(GLuint shaderProgram);

    void moveForward(GLfloat deltaTime);
    void moveBackward(GLfloat deltaTime);
    void moveLeft(GLfloat deltaTime);
    void moveRight(GLfloat deltaTime);
    
    void moveMouse(GLfloat deltaTime, GLint &mouseX, GLint &mouseY, GLuint mainWinW, GLuint mainWinH);
    
    void setFirstTime(){mouseFirst = true;}
private:
    /* Camera position front up */
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    
    // keyboard camera move sensitivity
    GLfloat keysSensisivity = 3.4f;
    // mouse sensistivity
    GLfloat mouseSensitivity = 0.55f;
    
    bool mouseFirst;
    GLfloat mouseLastX;
    GLfloat mouseLastY;
    GLfloat yaw;
    GLfloat pitch;
    
    glm::mat4 view;
    glm::mat4 projection;
};

#endif//CAMERA
