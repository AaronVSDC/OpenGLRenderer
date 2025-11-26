#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "glad/glad.h"
#include "iostream"
#include "Window.h"
#include <glm/glm.hpp>

namespace Papyrus
{
    Renderer::Renderer() {}

    Renderer::~Renderer()
    {
    }

    void Renderer::render() const
    {
        //TODO: NEEDS TO BE DECOUPLED AND JUST BOILERPLATE
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene* scene = SceneManager::getInstance().getCurrentScene();
        if (!scene) return;

        CameraComponent* camera = scene->getActiveCamera();
        if (!camera)
        {
            std::cout << "WARNING: No active camera in scene!";
            return;
        }

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 proj = camera->getProjectionMatrix();

        for (auto& gameObject : scene->getGameObjects())
        {
            auto mesh = gameObject->getComponent<MeshComponent>();
            auto material = gameObject->getComponent<MaterialComponent>();

            if (!mesh || !material) {
                continue;
            }


            OpenGLShader* shader = material->getShader();
            Texture* texture = material->getTexture();

            shader->use();
            //TODO: INSTEAD OF JUST ADDING THIS HERE LIKE A LAZY PERSON ACTUALLY DECOUPLE THE RENDERING LOGIC AND MAKE IT COMPLETELY BOILERPLATE
            const glm::vec3 lightDir = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
            shader->uploadUniformFloat3("lightDirection", lightDir);
            shader->uploadUniformFloat3("lightColor", glm::vec3(1.000, 0.75, 0.45));
            shader->uploadUniformFloat3("ambientColor", glm::vec3(0.2f));

            // Upload MVP
            shader->uploadUniformMat4("model", gameObject->transform.getModelMatrix());
            shader->uploadUniformMat4("view", view);
            shader->uploadUniformMat4("projection", proj);

            // Bind texture
            if (texture)
            {
                texture->bind(0);
            }
             
            // Draw VAO
            glBindVertexArray(mesh->getVAO());
            glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);

        }
        SDL_GL_SwapWindow(GWindow->getHandle());

    }

}