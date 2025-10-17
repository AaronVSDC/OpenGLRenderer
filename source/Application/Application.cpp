#include "Application.h"
#include "Window/Window.h"
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
        bool quit = false; 
        while (!quit)
        {
            SDL_Event sdlEvent;

            while (SDL_PollEvent(&sdlEvent))
            {
                if (sdlEvent.type == SDL_EVENT_QUIT) 
                {
                    quit = true;
                }
            }
        }

	}
}