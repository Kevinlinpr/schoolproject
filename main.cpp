#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

/* glm headers */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "errors.hpp"
#include "shader.hpp"
#include "Grid.hpp"
#include "Camera.hpp"
#include "skinned_mesh.hpp"
#include "GUI.hpp"
#include "Model.hpp"

// whether to quit main loop
bool quit = false;

// SDL window context
SDL_Window *mainWindow = NULL;
SDL_GLContext mainGLContext;

/* Window params */
const GLchar *mainWinTitle = "App to test OpenGL";
GLuint mainWinH = 600;
GLuint mainWinW = 800;
GLuint mainWinPosX = 100;
GLuint mainWinPosY = 100;

// Initialise all the app needs 
void initAll();
// Read SDL2 input events 
void readInputEvents();
// Load all shaders
void loadShaders();
// rendering
void rendering();
// clear mem
void clear();

// for main loop's iteration time calculation
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat currentTime = 0.0f;

// whether the mouse is captured
bool mouseCaptured = false;

// shader program of the animated models
GLuint shaderProgramAnimation = 0;
// shder for grid
GLuint shaderProgramGrid = 0;

Grid *grid;
Camera *camera;
//SkinnedMesh m_mesh;
//SkinnedMesh m_mesh2;

// vector of all models
std::vector<Model> models;

GUI* gui;

// ambient color light
glm::vec3 ambientLightColor(0.6, 0.0, 0.6);
// ambient light strength
GLfloat ambientLightStrength = 0.2f;

// diffuse light source position
glm::vec3 diffusePosition(0.0f, 200.0f, 0.0f);
glm::vec3 diffuseColor(0.5f, 0.5f, 0.5f);

int main(int argc, char *argv[])
{
    initAll();
    
    loadShaders();
    
    while(!quit)
    {
        currentTime = (GLfloat)SDL_GetTicks() / 1000;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
        
        rendering();
        
        SDL_GL_SwapWindow(mainWindow);
        
        readInputEvents();
    }
    
    clear();
    return 0;
}

void clear()
{
    delete grid;
    delete camera;
    delete gui;
}

void readInputEvents()
{
    const Uint8 *keysStates = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event))
    {
        if(!mouseCaptured)
            gui->injectInput(event);
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_c)
        {
            mouseCaptured = !mouseCaptured;
            camera->setFirstTime();
        }
        if(mouseCaptured)
        {
            if (keysStates[SDL_SCANCODE_W])
                camera->moveForward(deltaTime);
            if (keysStates[SDL_SCANCODE_S])
                camera->moveBackward(deltaTime);
            if (keysStates[SDL_SCANCODE_A])
                camera->moveLeft(deltaTime);
            if (keysStates[SDL_SCANCODE_D])
                camera->moveRight(deltaTime);
        
            // Camera
            int mouseX = 0, mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            camera->moveMouse(deltaTime, mouseX, mouseY, mainWinW, mainWinH);
            SDL_WarpMouseInWindow(mainWindow, mouseX, mouseY);
            // end Camera
        }
        
        if (event.type == SDL_QUIT)
		quit = 1;
    }
}

void rendering()
{
    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgramAnimation);

    GLuint diffusePositionLoc = glGetUniformLocation(shaderProgramAnimation, "lightPos");
    glUniform3fv(diffusePositionLoc, 1, glm::value_ptr(diffusePosition));
    
    // diffuse light color
    GLuint diffuseColorLoc = glGetUniformLocation(shaderProgramAnimation, "lightColor");
    glUniform3fv(diffuseColorLoc, 1, glm::value_ptr(diffuseColor));
    
    // ambient light color
    GLuint ambientLightColorLoc = glGetUniformLocation(shaderProgramAnimation, "ambientColor");
    glUniform3fv(ambientLightColorLoc, 1, glm::value_ptr(ambientLightColor));
    // ambient color strength
    GLuint ambientLightStrLoc = glGetUniformLocation(shaderProgramAnimation, "ambientStrength");
    glUniform1f(ambientLightStrLoc, ambientLightStrength);
    
    /* Model matrix */
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        
        glm::quat rotate();
        //============================
        
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        
    /* Set uniform model matrix */
		GLuint modeleLoc = glGetUniformLocation(shaderProgramAnimation, "model");
		glUniformMatrix4fv(modeleLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    //m_mesh.setBoneTransformations(shaderProgramAnimation, currentTime);
        
    camera->calcCameraMatrices(mainWinW, mainWinH);
    camera->setMatrices(shaderProgramAnimation);
    //m_mesh.render(shaderProgramAnimation);
    
    for(unsigned int i = 0; i < models.size(); ++i)
    {
        models[i].setBoneTransformations(shaderProgramAnimation, currentTime);
        models[i].calcAndSetModelMatrix(shaderProgramAnimation);
        models[i].render(shaderProgramAnimation);
    }
    
    ///////////////////////////////////////////////////
    // Model 2 matrix 
		/*glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(10.0f, 00.0f, 20.0f)); // Translate it down a bit so it's at the center of the scene
		model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        */
    // Set uniform model matrix 
		/*GLuint modeleLoc2 = glGetUniformLocation(shaderProgramAnimation, "model");
		glUniformMatrix4fv(modeleLoc2, 1, GL_FALSE, glm::value_ptr(model2));    
        */
    //m_mesh2.setBoneTransformations(shaderProgramAnimation, currentTime);
    
    //camera->calcCameraMatrices(mainWinW, mainWinH);
    //camera->setMatrices(shaderProgramAnimation);
    //m_mesh2.render(shaderProgramAnimation);
    glUseProgram(0);
    
    glUseProgram(shaderProgramGrid);
    camera->calcCameraMatrices(mainWinW, mainWinH);
    camera->setMatrices(shaderProgramGrid);
    grid->render(shaderProgramGrid);
    glUseProgram(0);
    
    gui->render();
}

void initAll()
{
    /* Init SDL2 */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		fatalError("Could not initialize SDL!\n");

	/* Set attributes OpenGL 4.5 */
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	/* Create window */
	mainWindow = SDL_CreateWindow(mainWinTitle, mainWinPosX, mainWinPosY, mainWinW, mainWinH, SDL_WINDOW_OPENGL);
	if (mainWindow == NULL)
		fatalError("Could not create SDL window!\n");

	/* Create OpenGL context */
	mainGLContext = SDL_GL_CreateContext(mainWindow);
	if (mainGLContext == NULL)
		fatalError("Could not create the OpenGL context!\n");

    if(glewInit() != GLEW_OK)
        std::cout << "error GLEW init" << std::endl;
    
	/* Set viweport */
	GLint vpWidth, vpHeight;
	SDL_GL_GetDrawableSize(mainWindow, &vpWidth, &vpHeight);
	glViewport(0, 0, vpWidth, vpHeight);
    
    glEnable(GL_DEPTH_TEST);
    
    printf("Vendor:          %s\n",glGetString(GL_VENDOR));
	printf("Renderer:        %s\n", glGetString(GL_RENDERER));
	printf("Version OpenGL:  %s\n", glGetString(GL_VERSION));
	printf("Version GLSL:    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // init grid object
    grid = new Grid();
    
    // init camera object
    camera = new Camera();
    
    // init gui
    gui = new GUI();
    gui->setModelsPointer(models);
    // init m_mesh
    // models/ArmyPilot/ArmyPilot.dae
    // models/robot.FBX
    //m_mesh.loadMesh("models/robot.FBX");
    //m_mesh2.loadMesh("models/ArmyPilot/ArmyPilot.dae");
    
    //Model new_model("models/robot.FBX");
    //models.push_back(new_model);
}

void loadShaders()
{
    // load shader for animation
    std::cout << "Load animation shaders" << std::endl;
	GLuint vertexShader = compileShader("Shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader("Shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	shaderProgramAnimation = createProgram();
	attachShader(vertexShader, shaderProgramAnimation);
	attachShader(fragmentShader, shaderProgramAnimation);
	linkProgram(shaderProgramAnimation);
    deleteShader(fragmentShader);
    deleteShader(vertexShader);
    
    // load greed shader
    std::cout << "Load grin shaders" << std::endl;
    vertexShader = compileShader("Shaders/vertexGrid.glsl", GL_VERTEX_SHADER);
	fragmentShader = compileShader("Shaders/fragmentGrid.glsl", GL_FRAGMENT_SHADER);
	shaderProgramGrid = createProgram();
	attachShader(vertexShader, shaderProgramGrid);
	attachShader(fragmentShader, shaderProgramGrid);
	linkProgram(shaderProgramGrid);
    deleteShader(fragmentShader);
    deleteShader(vertexShader);
}
