#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include "Singleton.h"

//std
#include <string> 
#include <memory>

namespace Papyrus
{

	class Window final
	{
	public:
		Window(const std::string& title, int width, int height); 
		~Window(); 
		SDL_Window* getHandle() const { return m_SDLWindow; }

		const int getHeight() const { return m_HEIGHT;  }
		const int getWidht() const { return m_WIDTH;  }

	private:
		SDL_Window* m_SDLWindow = {nullptr};
		SDL_GLContext m_Context = {}; 

		const int m_WIDTH = 800; 
		const int m_HEIGHT = 600; 
	};

	extern std::unique_ptr<Window> GWindow;  

	 
}


#endif




