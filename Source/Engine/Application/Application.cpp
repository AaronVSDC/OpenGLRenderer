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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "GameObject.h"
#include "Camera.h" 
#include "SceneManager.h" 
#include "Renderer.h"
#include <chrono>
#include <thread>

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

    struct SimpleMesh {

        std::vector<float> vertices;

    };

    SimpleMesh loadFBX(const std::string& path)
    {
        SimpleMesh mesh;

        // Create an importer instance
        Assimp::Importer importer;

        // Load the FBX file
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenNormals);

        // Check for errors
        if (!scene || !scene->HasMeshes()) {
            std::cerr << "Failed to load model: " << importer.GetErrorString() << std::endl;
            return mesh;
        }

        const aiMesh* aMesh = scene->mMeshes[0]; // first mesh

        // Loop through faces to create sequential triangle vertices
        for (unsigned int f = 0; f < aMesh->mNumFaces; ++f) {
            const aiFace& face = aMesh->mFaces[f];
            // Process indices
            for (unsigned int i = 0; i < face.mNumIndices; ++i) {
                unsigned int idx = face.mIndices[i];

                // position
                mesh.vertices.push_back(aMesh->mVertices[idx].x);
                mesh.vertices.push_back(aMesh->mVertices[idx].y);
                mesh.vertices.push_back(aMesh->mVertices[idx].z);

                // texcoords
                if (aMesh->HasTextureCoords(0)) {
                    mesh.vertices.push_back(aMesh->mTextureCoords[0][idx].x);
                    mesh.vertices.push_back(aMesh->mTextureCoords[0][idx].y);
                }
                else {
                    mesh.vertices.push_back(0.0f);
                    mesh.vertices.push_back(0.0f);
                }
            }
        }

        return mesh;
    }


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
        //LOAD FBX MODEL
        //------------------------

        //SimpleMesh mesh = loadFBX("Resources/Models/monkey.fbx");
        SimpleMesh mesh = loadFBX("Resources/Models/cottage.fbx");
        //SimpleMesh mesh = loadFBX("Resources/Models/HLboot_camp.fbx"); 
        if (mesh.vertices.empty()) {
            std::cerr << "No vertices loaded." << std::endl;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Error", "Failed to load FBX.", m_Window->getHandle());
        }

        //--------------------------------------
        //VERTEX BUFFER AND ATTRIBUTES
        //--------------------------------------
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //Mesh Vertices
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

        //Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //Texcoords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //--------------------
        // TEXTURES
        //--------------------
        auto awesomeFaceTexture = std::make_unique<Texture>("Resources/Textures/snow.png");
        auto containerTexture = std::make_unique<Texture>("Resources/Textures/container.jpg");

        shaders->use();
        shaders->uploadUniformInt("snowTexture", 0);
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

            containerTexture->bind(0);
            awesomeFaceTexture->bind(1); 
            shaders->use();

            glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            shaders->uploadUniformMat4("projection", projection);
            shaders->uploadUniformMat4("view", view);


            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(1.0f)); // scale
            shaders->uploadUniformMat4("model", model);

            glBindVertexArray(VAO);

            //mesh vertices
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size() / 5);

            SDL_GL_SwapWindow(m_Window->getHandle());
        }



        //------------------------------------------------------
        load();

        constexpr int desiredFPS{ 60 };
        constexpr int frameTimeMs{ 1000 / desiredFPS };

        auto& renderer = Renderer::getInstance();
        auto& sceneManager = SceneManager::getInstance();
        auto& input = InputManager::getInstance();

        const float fixedTimeStep{ 0.02f };

        bool doContinue = true;
        auto lastTime = std::chrono::high_resolution_clock::now();
        float lag = 0.0f;

        sceneManager.onEnable();
        sceneManager.start();
        while (doContinue)
        {
            const auto currentTime = std::chrono::high_resolution_clock::now();
            const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            lag += deltaTime;

            doContinue = input.processInput();

            sceneManager.onEnable();

            while (lag >= fixedTimeStep)
            {
                sceneManager.fixedUpdate(fixedTimeStep);
                lag -= fixedTimeStep;
            }
            sceneManager.update(deltaTime);
            renderer.render();
            sceneManager.onDisable();

            const auto sleepTime = currentTime + std::chrono::milliseconds(frameTimeMs) - std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(sleepTime);
        }


        //-----------------------------------------------------------------------------








        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    void Application::load()
    {
        auto camera = std::unique_ptr<GameObject>(); 

        camera->addComponent(std::unique_ptr<Camera>()); 


    }
}
