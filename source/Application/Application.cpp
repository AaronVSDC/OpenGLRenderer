#include "Application.h"
#include "Window.h"
#include "InputManager.h"
#include "OpenGLShader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <SDL3/SDL_mouse.h>
#include "Texture.h"


namespace Papyrus
{
    // Camera variables
    static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    static bool firstMouse = true;
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    static float lastX = 400.0f;
    static float lastY = 300.0f;
    static float fov = 45.0f;

    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    Application::Application()
    {
        m_Window = std::make_unique<Window>("Portal!!", 800, 600);
        SDL_SetWindowRelativeMouseMode(m_Window->getHandle(), true); 
    }

    Application::~Application() {}

    void Application::run()
    {
        InputManager& inputManager = InputManager::getInstance();

        //--------------------
        //SHADER CREATION
        //--------------------
        auto shaders = std::make_unique<OpenGLShader>(
            "Shaders/vertShader.vert",
            "Shaders/fragShader.frag"
        );

        //this lines enables depth testing so that stuff behind other stuff isnt visible
        glEnable(GL_DEPTH_TEST);

        //------------------------
        //VERTICES INPUT (will be done with modelloading)
        //------------------------
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
        };


        //--------------------------------------
        //VERTEX BUFFER AND VERTEX ATTRIBUTES
        //--------------------------------------
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //--------------------
        // TEXTURES
        //--------------------
        auto awesomeFaceTexture = std::make_unique<Texture>("Resources/Textures/awesomeface.png");
        auto containerTexture = std::make_unique<Texture>("Resources/Textures/container.jpg");

        shaders->use();
        shaders->uploadUniformInt("awesomefaceTexture", 0);
        shaders->uploadUniformInt("containerTexture", 1);

        bool doContinue = true;
        while (doContinue)
        {
            float currentFrame = SDL_GetTicks() / 1000.0f;
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Handle SDL input
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT) doContinue = false;
                if (e.type == SDL_EVENT_WINDOW_RESIZED)
                    glViewport(0, 0, e.window.data1, e.window.data2);

                if (e.type == SDL_EVENT_MOUSE_MOTION)
                {

                    float xoffset = static_cast<float>(e.motion.xrel);
                    float yoffset = static_cast<float>(e.motion.yrel);

                    float sensitivity = 0.1f;
                    xoffset *= sensitivity;
                    yoffset *= sensitivity;

                    yaw += xoffset;
                    pitch -= yoffset;

                    if (pitch > 89.0f) pitch = 89.0f;
                    if (pitch < -89.0f) pitch = -89.0f;

                    glm::vec3 front;
                    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front.y = sin(glm::radians(pitch));
                    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    cameraFront = glm::normalize(front);
                }

                if (e.type == SDL_EVENT_MOUSE_WHEEL)
                {
                    fov -= (float)e.wheel.y;
                    if (fov < 1.0f) fov = 1.0f;
                    if (fov > 45.0f) fov = 45.0f;
                }
            }

            const bool* state = SDL_GetKeyboardState(nullptr); 
            float cameraSpeed = 2.5f * deltaTime;
            if (state[SDL_SCANCODE_W]) cameraPos += cameraSpeed * cameraFront;
            if (state[SDL_SCANCODE_S]) cameraPos -= cameraSpeed * cameraFront;
            if (state[SDL_SCANCODE_A]) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (state[SDL_SCANCODE_D]) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (state[SDL_SCANCODE_ESCAPE]) doContinue = false;

            // Render
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            awesomeFaceTexture->bind(0); 
            containerTexture->bind(1); 
            shaders->use();

            glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            shaders->uploadUniformMat4("projection", projection);
            shaders->uploadUniformMat4("view", view);

            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shaders->uploadUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            SDL_GL_SwapWindow(m_Window->getHandle());
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}
