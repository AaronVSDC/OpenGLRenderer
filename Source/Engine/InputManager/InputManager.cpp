#include "InputManager.h"
#include <SDL3/SDL.h>
#include <glad\glad.h>

namespace Papyrus
{
    bool InputManager::processInput()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return false;
            //TODO: maybe its best to seperate the openGL stuff from the actual processInput and other engine code (if we plan on making the engine code api agnostic)
            if (e.type == SDL_EVENT_WINDOW_RESIZED) 
                glViewport(0, 0, e.window.data1, e.window.data2);
            
        }
        return true; 
    }
}
