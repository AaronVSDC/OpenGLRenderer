#include "Application.h"
#include "Window/Window.h"
#include "InputManager.h"
#include "glad/glad.h"

namespace Papyrus
{
	Application::Application()
	{
		m_Window = std::make_unique<Window>("Portal!!", 800, 600); 
	}
    Application::~Application()
    {
    }
    void Application::run()
	{
        InputManager& inputManager = InputManager::getInstance(); 

        bool doContinue = true; 
        while (doContinue)
        {
            doContinue = inputManager.processInput(); 

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            SDL_GL_SwapWindow(m_Window->getHandle());

        }

	}
}