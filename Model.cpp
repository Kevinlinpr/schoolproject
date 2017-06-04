#include "Model.hpp"

Model::Model() :
       translate(0.0f, 0.0f, 0.0f),
       rotate(0.0f, 0.0f, 0.0f),
       scale(0.0f, 0.0f, 0.0f),
       currentAnimation(0),
       numAnimations(0),
       visible(true)
   {
       
   }
    Model::~Model()
    {
        //delete mesh;
    }
    bool Model::loadMesh(std::string path)
    {
       mesh = new SkinnedMesh();
       bool ret = mesh->loadMesh(path.c_str());
       if(ret) 
          numAnimations = mesh->getNumAnimations();
       return ret;
    }
    void Model::render(GLuint shaderProgram)
    {
        if(visible == false) return;
        mesh->render(shaderProgram);
    }
    void Model::setBoneTransformations(GLuint shaderProgram, GLfloat currentTime)
    {
        if(numAnimations == 0) return;
        mesh->setBoneTransformations(shaderProgram, currentTime);
    }
    void Model::calcAndSetModelMatrix(GLuint shaderProgram)
    {
        if(visible == false) return;
        
        glm::mat4 model;
        if(translate != glm::vec3(0.0f, 0.0f, 0.0f))
            // translate
            model = glm::translate(model, translate);
        if(rotate.x != 0.0f)
            // rotate X
            model = glm::rotate(model, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
        if(rotate.y != 0.0f)
            // rotate Y
            model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
        if(rotate.z != 0.0f)
            // rotate Z
            model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
        if(scale != glm::vec3(0.0f, 0.0f, 0.0f))
        // scale
        model = glm::scale(model, scale);
        
        // set uniform model matrix
        GLuint modeleLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modeleLoc, 1, GL_FALSE, glm::value_ptr(model));
    }
