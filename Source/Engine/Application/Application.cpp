#include "Application.h"
#include "Window.h"
#include "InputManager.h"
#include "OpenGLShader.h"
#include <iostream>
#include "Texture.h"
#include "GameObject.h"
#include "CameraComponent.h" 
#include "SceneManager.h" 
#include "Renderer.h"
#include <chrono>
#include <thread>
#include "MeshComponent.h"
#include "MaterialComponent.h"

namespace Papyrus
{
  
    Application::Application()
    {
        GWindow = std::make_unique<Window>("Portal!!", 800, 600);
        SDL_SetWindowRelativeMouseMode(GWindow->getHandle(), true); 
    }

    Application::~Application() {}

    void Application::run()
    {
        load(); 

        constexpr int desiredFPS{ 144 }; 
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

            while (lag >= fixedTimeStep)
            {
                sceneManager.fixedUpdate(fixedTimeStep);
                lag -= fixedTimeStep;
            }
            sceneManager.update(deltaTime);
            renderer.render();
             
            const auto sleepTime = currentTime + std::chrono::milliseconds(frameTimeMs) - std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(sleepTime);
        }
        sceneManager.onDisable();


    }
    void Application::load()
    {
        auto camera = std::make_unique<GameObject>(); 
        camera->addComponent(std::make_unique<CameraComponent>()); 
        camera->transform.position = glm::vec3{ 0.f,3.f,10.f }; 

        auto portalButton = std::make_unique<GameObject>(); 
        portalButton->addComponent(std::make_unique<MeshComponent>("Resources/Models/portal2_button.fbx"));
        portalButton->transform.scale = glm::vec3{ 0.1 }; 

        auto portalButton2 = std::make_unique<GameObject>();
        portalButton2->addComponent(std::make_unique<MeshComponent>("Resources/Models/portal2_button.fbx"));
        portalButton2->transform.scale = glm::vec3{ 0.1 };
        portalButton2->transform.position = glm::vec3{ -30.f,0.f,0.f }; 
        portalButton2->addComponent(std::make_unique<MeshComponent>("Resources/Models/portal2_button.fbx"));


        auto shader = std::make_unique<OpenGLShader>("Shaders/vertShader.vert", "Shaders/fragShader.frag"); 
        auto texture = std::make_unique<Texture>("Resources/Textures/portal_button_blue.jpeg"); 
        auto shader2 = std::make_unique<OpenGLShader>("Shaders/vertShader.vert", "Shaders/fragShader.frag");
        auto texture2 = std::make_unique<Texture>("Resources/Textures/portal_button_blue.jpeg");
        portalButton->addComponent(std::make_unique<MaterialComponent>(std::move(shader), std::move(texture)));
        portalButton2->addComponent(std::make_unique<MaterialComponent>(std::move(shader2), std::move(texture2))); 

        auto* scene = SceneManager::getInstance().createScene("Default");
        if (scene != nullptr) 
        {
            scene->setIsActive(true);
            scene->add(std::move(camera));
            scene->add(std::move(portalButton));
            scene->add(std::move(portalButton2));

        }

    }
}
