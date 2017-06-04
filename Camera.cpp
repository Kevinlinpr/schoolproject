#include "Camera.hpp"

Camera::Camera() :
    cameraPos(glm::vec3(0.0f, 0.0f, 0.0f)),
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
    cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    mouseFirst(true),
    mouseLastX(0.0f),
    mouseLastY(0.0f),
    yaw(-90.0f),
    pitch(0.0f)
{
    
}

void Camera::setMatrices(GLuint shaderProgram)
{
    /* Set uniform view matrix */
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	/* Set uniform projection matrix */
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::calcCameraMatrices(GLuint mainWinW, GLuint mainWinH)
{
    /* View matrix */
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	/* >> camera change*/

	/* Projection matrix */
	projection = glm::perspective<float>(float(45.0f * (PI / 180.0f)), mainWinW / mainWinH, 1.0f, 1000.0f);
}

void Camera::moveForward(GLfloat deltaTime)
{
    GLfloat currentCamSpeed = keysSensisivity * deltaTime;
    cameraPos += currentCamSpeed * cameraFront;
}

void Camera::moveBackward(GLfloat deltaTime)
{
    GLfloat currentCamSpeed = keysSensisivity * deltaTime;
    cameraPos -= currentCamSpeed * cameraFront;
}

void Camera::moveLeft(GLfloat deltaTime)
{
    GLfloat currentCamSpeed = keysSensisivity * deltaTime;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCamSpeed;
}

void Camera::moveRight(GLfloat deltaTime)
{
    GLfloat currentCamSpeed = keysSensisivity * deltaTime;
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCamSpeed; 
}

void Camera::moveMouse(GLfloat deltaTime, GLint &mouseX, GLint &mouseY, GLuint mainWinW, GLuint mainWinH)
{
    if(mouseFirst)
    {
        mouseLastX = (float)mouseX;
        mouseLastY = (float)mouseY;
        mouseFirst = false;
    }
    
    GLfloat mouseOffsetX = mouseX - mouseLastX;
    GLfloat mouseOffsetY = mouseY - mouseLastY;
    
    mouseOffsetX *= mouseSensitivity;
    mouseOffsetY *= mouseSensitivity;
     
    yaw += mouseOffsetX;
    pitch += mouseOffsetY;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
 
    if(mouseX > mainWinW - 30)
        mouseX = 40;
    if(mouseX < 30)
        mouseX = mainWinW - 40;
    if(mouseY > mainWinH - 30 /* && pitch < 89.0f*/)
        mouseY = 40;
    if(mouseY < 30  /*&& pitch > -89.0f*/)
        mouseY = mainWinH - 40;
    
    mouseLastX = (float)mouseX;
    mouseLastY = (float)mouseY;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(-pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
