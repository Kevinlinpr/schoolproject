#ifndef GUI_HEADER
#define GUI_HEADER

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include <cegui-0/CEGUI/CEGUI.h>
#include <cegui-0/CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include "Model.hpp"

class GUI
{
public:
    GUI();
    void injectInput(SDL_Event event);
    
    void setModelsPointer(std::vector<Model>& models);
    bool loadMesh(std::string path);
    
    void render();
private:
    void createWindows();
    void createLoadWindow();
    void createAlertWindow();
    void createPropertiesWindow();
    
    
    void alertWindowSetMessage(std::string m);
    bool loadFileEvent(const CEGUI::EventArgs& e);
    bool alertOKEvent(const CEGUI::EventArgs& e);
    
    
    CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton);
    CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key);
    
    CEGUI::Window* rootWindow, *loadWindow, *alertWindow, *propertiesWindow;
    CEGUI::Window* alert;
    
    std::vector<Model> *models;
    
    static bool ceguiInitialized;
    static CEGUI::OpenGL3Renderer *myRenderer;  
    static CEGUI::DefaultResourceProvider *rp;
    static CEGUI::GUIContext *context;
};

#endif//GUIHEADER
