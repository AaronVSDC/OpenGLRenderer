#include <iostream>
#include <SDL3/SDL.h>
#include <glad/glad.h>

int main()
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", 800, 600, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		std::cout << "Failed to create SDL Window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cout << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DestroyContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}

	float vertices[] = {
	  0.0f,  0.5f,  // Vertex 1 (X, Y)
	  0.5f, -0.5f,  // Vertex 2 (X, Y)
	 -0.5f, -0.5f   // Vertex 3 (X, Y)
	};

	// Generate and bind VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1. Generate buffer ID
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// 2. Bind buffer to GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 3. Copy vertex data into buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const char* vertexShaderSource = R"glsl(
		#version 330 core
		layout (location = 0) in vec2 position;
		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";

	// Create, attach source, and compile
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const char* fragmentShaderSource = R"glsl(
		#version 330 core
		out vec4 outColor;
		void main()
		{
			outColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);  // Orange color
		}
	)glsl";

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Don't forget error checking here too!
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create program
	GLuint shaderProgram = glCreateProgram();

	// Attach shaders and link
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Clean up individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	SDL_Event windowEvent;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw 3 vertices as triangle


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DestroyContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}