#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

//std
#include <string>

namespace Papyrus
{

	class Window final
	{
	public:
		Window(const std::string& title, int width, int height);
		Window(const Window&) = delete;
		Window(const Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(const Window&&) = delete;
		~Window();

		SDL_Window* getHandle() const { return m_SDLWindow; }

	private:
		SDL_Window* m_SDLWindow; 
		SDL_GLContext m_Context; 
	};



}


#endif




