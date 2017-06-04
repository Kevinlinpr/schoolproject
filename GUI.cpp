#include "GUI.hpp"

GUI::GUI()
{
    if(!ceguiInitialized)
    {
        myRenderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

        rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
  
        // associate the default resource groups with their paths 
        rp->setResourceGroupDirectory( "schemes", "datafiles/schemes/" );
        rp->setResourceGroupDirectory( "imagesets", "datafiles/imagesets/" );
        rp->setResourceGroupDirectory( "fonts", "datafiles/fonts/" );
        rp->setResourceGroupDirectory( "layouts", "datafiles/layouts/" );
        rp->setResourceGroupDirectory( "looknfeel", "datafiles/looknfeel/" );
        rp->setResourceGroupDirectory( "lua_scripts", "datafiles/lua_scripts/" );
    
        // load default resource groups 
        CEGUI::ImageManager::setImagesetDefaultResourceGroup( "imagesets" );
        CEGUI::Font::setDefaultResourceGroup( "fonts" );
        CEGUI::Scheme::setDefaultResourceGroup( "schemes" );
        CEGUI::WidgetLookManager::setDefaultResourceGroup( "looknfeel" );
        CEGUI::WindowManager::setDefaultResourceGroup( "layouts" );
        CEGUI::ScriptModule::setDefaultResourceGroup( "lua_scripts" );
    
        // Create the context 
        context = &CEGUI::System::getSingleton().createGUIContext( myRenderer->getDefaultRenderTarget() );
    
        // Load scheme 
        CEGUI::SchemeManager::getSingleton().createFromFile( "WindowsLook.scheme" );
    
        // Load and set the font 
        CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font" );
        context->setDefaultFont( "DejaVuSans-10" );

        rootWindow =      CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
        context->setRootWindow(rootWindow);
        std::cout << "aaaa" << std::endl;
        createWindows();
    }
}

void GUI::createWindows()
{
   /* std::cout << "create alert win" << std::endl;
  alert = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/FrameWindow", "alert");
  alert->setPosition(CEGUI::UVector2( CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
  alert->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 100.0f), CEGUI::UDim(0.0f, 100.0f) ) );
  rootWindow->addChild(alert);
  
  CEGUI::Window *button = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Button", "button1" );
  alert->addChild( button );
  button->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.0f, 0.0f) ) );
  button->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 50.0f), CEGUI::UDim(0.0f, 30.0f) ) );
  button->setText( "fffffff" );*/
   
    createLoadWindow();
    createAlertWindow();
    createPropertiesWindow();
}

void GUI::createPropertiesWindow()
{
  propertiesWindow = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/FrameWindow", "properties");
  propertiesWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.05f, 0.0f), CEGUI::UDim(0.3f, 0.0f)));
  propertiesWindow->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 285.0f), CEGUI::UDim(0.0f, 400.0f) ) );
  propertiesWindow->setText("Models' properties");
  rootWindow->addChild(propertiesWindow);
    
CEGUI::Listbox *modelsList = static_cast<CEGUI::Listbox*> (CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Listbox", "modelsList" ));
  propertiesWindow->addChild(modelsList);
    modelsList->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 20.0f), CEGUI::UDim(0.0f, 20.0f) ) );
    modelsList->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 150.0f), CEGUI::UDim(0.0f, 30.0f) ) );
    
}

void GUI::createAlertWindow()
{
  alertWindow = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/FrameWindow", "alert");
  alertWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
  alertWindow->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 285.0f), CEGUI::UDim(0.0f, 100.0f) ) );
  alertWindow->setText("Alert");
  rootWindow->addChild(alertWindow);
  
CEGUI::Window *alertMessage = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Label", "alertMessage" );
  alertWindow->addChild(alertMessage);
    alertMessage->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 20.0f), CEGUI::UDim(0.0f, 20.0f) ) );
    alertMessage->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 150.0f), CEGUI::UDim(0.0f, 30.0f) ) );
    
    
    CEGUI::Window *buttonOK = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Button", "buttonOK" );
  alertWindow->addChild( buttonOK );
  buttonOK->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 190.0f), CEGUI::UDim(0.0f, 20.0f) ) );
  buttonOK->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 65.0f), CEGUI::UDim(0.0f, 30.0f) ) );
  buttonOK->setText( "OK" );
  
  buttonOK->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::alertOKEvent, this));
  
  alertWindow->setVisible(false);
}

bool GUI::alertOKEvent(const CEGUI::EventArgs& e)
{
    alertWindow->setVisible(false);
}

void GUI::alertWindowSetMessage(std::string m)
{
    alertWindow->getChild("alertMessage")->setText(m.c_str());
}

void GUI::createLoadWindow()
{
  loadWindow = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/FrameWindow", "load");
  loadWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.05f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));
  loadWindow->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 285.0f), CEGUI::UDim(0.0f, 100.0f) ) );
  loadWindow->setText("Load file");
  rootWindow->addChild(loadWindow);
    
  CEGUI::Window *editboxLoad = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Editbox", "editboxLoad" );
  loadWindow->addChild(editboxLoad);
    editboxLoad->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 20.0f), CEGUI::UDim(0.0f, 20.0f) ) );
    editboxLoad->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 150.0f), CEGUI::UDim(0.0f, 30.0f) ) );
    

    CEGUI::Window *buttonLoad = CEGUI::WindowManager::getSingleton().createWindow( "WindowsLook/Button", "buttonLoad" );
  loadWindow->addChild( buttonLoad );
  buttonLoad->setPosition( CEGUI::UVector2( CEGUI::UDim(0.0f, 190.0f), CEGUI::UDim(0.0f, 20.0f) ) );
  buttonLoad->setSize( CEGUI::USize( CEGUI::UDim(0.0f, 65.0f), CEGUI::UDim(0.0f, 30.0f) ) );
  buttonLoad->setText( "Load file" );
  
  buttonLoad->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUI::loadFileEvent, this));
}

bool GUI::loadFileEvent(const CEGUI::EventArgs& e)
{
    Model new_model;
    bool loaded = new_model.loadMesh(loadWindow->getChild("editboxLoad")->getText().c_str());
    if(loaded)
    {    
        models->push_back(new_model);
        CEGUI::ListboxTextItem *item = new CEGUI::ListboxTextItem(loadWindow->getChild("editboxLoad")->getText());
        propertiesWindow->getChild("modelsList")->addItem(item);
        loadWindow->getChild("editboxLoad")->setText("");
    } else
    {
        // alert window with message "File could not be loaded."
        alertWindow->setVisible(true);
        alertWindow->activate();
        alertWindowSetMessage("File could not be loaded.");
        //loadWindow->getChild("editboxLoad")->setText("");
    }
}

void GUI::setModelsPointer(std::vector<Model>& models)
{
    this->models = &models;
}

void GUI::render()
{
    glDisable(GL_DEPTH_TEST);
    CEGUI::System::getSingleton().renderAllGUIContexts();
    glEnable(GL_DEPTH_TEST);
}

void GUI::injectInput(SDL_Event event)
{
    switch (event.type)
	    {
	    case SDL_MOUSEMOTION:
	      context->injectMousePosition(event.motion.x, event.motion.y);
	      break;
	    case SDL_KEYDOWN:
	      context->injectKeyDown(SDLKeyToCEGUIKey(event.key.keysym.sym));
	      break;
	    case SDL_KEYUP:
	      context->injectKeyUp(SDLKeyToCEGUIKey(event.key.keysym.sym));
	      break;
	    case SDL_TEXTINPUT:
	      /* TODO utf-32 converter from sdl to CEGUI */
	      context->injectChar(event.text.text[0]);
	      break;
	    case SDL_MOUSEBUTTONDOWN:
	      context->injectMouseButtonDown(SDLButtonToCEGUIButton(event.button.button));
	      break;
	    case SDL_MOUSEBUTTONUP:
	      context->injectMouseButtonUp(SDLButtonToCEGUIButton(event.button.button));
	      break;
	    }
}

bool GUI::ceguiInitialized = false;;
CEGUI::OpenGL3Renderer *GUI::myRenderer = NULL;  
CEGUI::DefaultResourceProvider *GUI::rp = NULL;
CEGUI::GUIContext *GUI::context = NULL;


CEGUI::Key::Scan GUI::SDLKeyToCEGUIKey(SDL_Keycode key)
{
  using namespace CEGUI;
  switch (key) {
  case SDLK_BACKSPACE:    return Key::Backspace;
  case SDLK_TAB:          return Key::Tab;
  case SDLK_RETURN:       return Key::Return;
  case SDLK_PAUSE:        return Key::Pause;
  case SDLK_ESCAPE:       return Key::Escape;
  case SDLK_SPACE:        return Key::Space;
  case SDLK_COMMA:        return Key::Comma;
  case SDLK_MINUS:        return Key::Minus;
  case SDLK_PERIOD:       return Key::Period;
  case SDLK_SLASH:        return Key::Slash;
  case SDLK_0:            return Key::Zero;
  case SDLK_1:            return Key::One;
  case SDLK_2:            return Key::Two;
  case SDLK_3:            return Key::Three;
  case SDLK_4:            return Key::Four;
  case SDLK_5:            return Key::Five;
  case SDLK_6:            return Key::Six;
  case SDLK_7:            return Key::Seven;
  case SDLK_8:            return Key::Eight;
  case SDLK_9:            return Key::Nine;
  case SDLK_COLON:        return Key::Colon;
  case SDLK_SEMICOLON:    return Key::Semicolon;
  case SDLK_EQUALS:       return Key::Equals;
  case SDLK_LEFTBRACKET:  return Key::LeftBracket;
  case SDLK_BACKSLASH:    return Key::Backslash;
  case SDLK_RIGHTBRACKET: return Key::RightBracket;
  case SDLK_a:            return Key::A;
  case SDLK_b:            return Key::B;
  case SDLK_c:            return Key::C;
  case SDLK_d:            return Key::D;
  case SDLK_e:            return Key::E;
  case SDLK_f:            return Key::F;
  case SDLK_g:            return Key::G;
  case SDLK_h:            return Key::H;
  case SDLK_i:            return Key::I;
  case SDLK_j:            return Key::J;
  case SDLK_k:            return Key::K;
  case SDLK_l:            return Key::L;
  case SDLK_m:            return Key::M;
  case SDLK_n:            return Key::N;
  case SDLK_o:            return Key::O;
  case SDLK_p:            return Key::P;
  case SDLK_q:            return Key::Q;
  case SDLK_r:            return Key::R;
  case SDLK_s:            return Key::S;
  case SDLK_t:            return Key::T;
  case SDLK_u:            return Key::U;
  case SDLK_v:            return Key::V;
  case SDLK_w:            return Key::W;
  case SDLK_x:            return Key::X;
  case SDLK_y:            return Key::Y;
  case SDLK_z:            return Key::Z;
  case SDLK_DELETE:       return Key::Delete;
  case SDLK_KP_PERIOD:    return Key::Decimal;
  case SDLK_KP_DIVIDE:    return Key::Divide;
  case SDLK_KP_MULTIPLY:  return Key::Multiply;
  case SDLK_KP_MINUS:     return Key::Subtract;
  case SDLK_KP_PLUS:      return Key::Add;
  case SDLK_KP_ENTER:     return Key::NumpadEnter;
  case SDLK_KP_EQUALS:    return Key::NumpadEquals;
  case SDLK_UP:           return Key::ArrowUp;
  case SDLK_DOWN:         return Key::ArrowDown;
  case SDLK_RIGHT:        return Key::ArrowRight;
  case SDLK_LEFT:         return Key::ArrowLeft;
  case SDLK_INSERT:       return Key::Insert;
  case SDLK_HOME:         return Key::Home;
  case SDLK_END:          return Key::End;
  case SDLK_PAGEUP:       return Key::PageUp;
  case SDLK_PAGEDOWN:     return Key::PageDown;
  case SDLK_F1:           return Key::F1;
  case SDLK_F2:           return Key::F2;
  case SDLK_F3:           return Key::F3;
  case SDLK_F4:           return Key::F4;
  case SDLK_F5:           return Key::F5;
  case SDLK_F6:           return Key::F6;
  case SDLK_F7:           return Key::F7;
  case SDLK_F8:           return Key::F8;
  case SDLK_F9:           return Key::F9;
  case SDLK_F10:          return Key::F10;
  case SDLK_F11:          return Key::F11;
  case SDLK_F12:          return Key::F12;
  case SDLK_F13:          return Key::F13;
  case SDLK_F14:          return Key::F14;
  case SDLK_F15:          return Key::F15;
  case SDLK_RSHIFT:       return Key::RightShift;
  case SDLK_LSHIFT:       return Key::LeftShift;
  case SDLK_RCTRL:        return Key::RightControl;
  case SDLK_LCTRL:        return Key::LeftControl;
  case SDLK_RALT:         return Key::RightAlt;
  case SDLK_LALT:         return Key::LeftAlt;
  case SDLK_SYSREQ:       return Key::SysRq;
  case SDLK_MENU:         return Key::AppMenu;
  case SDLK_POWER:        return Key::Power;
  default:                return Key::Unknown;
  }
}

CEGUI::MouseButton GUI::SDLButtonToCEGUIButton(Uint8 sdlButton)
{
  switch (sdlButton) {
  case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
  case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
  case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
  case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
  case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
  }
  return CEGUI::MouseButton::NoButton;
}
