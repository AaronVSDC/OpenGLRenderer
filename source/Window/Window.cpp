#include "Window.h"
#include <glad/glad.h>
#include <iostream>

namespace Papyrus
{
	Window::Window(const std::string& title, int width, int height)
	{
		if (!SDL_Init(SDL_INIT_VIDEO)) {
			std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		m_SDLWindow = SDL_CreateWindow(
			title.c_str(),
			width,
			height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		if (m_SDLWindow == nullptr) {
			std::cerr << "Failed to create SDL Window: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return;
		}

		m_Context = SDL_GL_CreateContext(m_SDLWindow);
		if (!m_Context) {
			std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
			return;
		}

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glViewport(0, 0, width, height);
	}

	Window::~Window()
	{
		SDL_GL_DestroyContext(m_Context);
		SDL_DestroyWindow(m_SDLWindow);
		SDL_Quit();
	}
}