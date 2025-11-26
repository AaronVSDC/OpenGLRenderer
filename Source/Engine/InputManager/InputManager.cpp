#include "InputManager.h"
#include <SDL3/SDL.h>
#include <glad\glad.h>

namespace Papyrus
{
    bool InputManager::processInput()
    {
        m_MouseDelta = {};   // reset each frame

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return false;
            
            if (e.type == SDL_SCANCODE_ESCAPE) 
                return false; 

            if (e.type == SDL_EVENT_MOUSE_MOTION)
            {
                m_MouseDelta.x += e.motion.xrel;
                m_MouseDelta.y += e.motion.yrel;
            }
        }

        return true;
    }
}
