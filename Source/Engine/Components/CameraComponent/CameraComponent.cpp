#include "CameraComponent.h"
#include "InputManager.h"
#include <SDL3/SDL_events.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h" 
#include "GameObject.h"
#include "Window.h"

namespace Papyrus
{
    CameraComponent::CameraComponent()
        : 
        m_Front(0.0f, 0.0f, -1.0f),
        m_Up(0.0f, 1.0f, 0.0f),
        m_Yaw(-90.0f),
        m_Pitch(0.0f),
        m_Fov(45.0f),
        m_LastX(400.0f),
        m_LastY(300.0f),
        m_FirstMouse(true)
    {
    }

    void CameraComponent::update(float deltaTime)
    { 
        //Todo: use command pattern instead for polling input

        Transform& transform = getOwner()->transform;

        // ---------- KEYBOARD ----------
        const bool* state = SDL_GetKeyboardState(nullptr);
        float speed = 2.5f * deltaTime;

        if (state[SDL_SCANCODE_W]) transform.position += speed * m_Front;
        if (state[SDL_SCANCODE_S]) transform.position -= speed * m_Front;
        if (state[SDL_SCANCODE_A]) transform.position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
        if (state[SDL_SCANCODE_D]) transform.position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;

        // ---------- MOUSE LOOK (FROM INPUT MANAGER) ----------
        auto& input = InputManager::getInstance();
        auto delta = input.getMouseDelta();

        float sensitivity = 0.1f;
        m_Yaw += delta.x * sensitivity;
        m_Pitch -= delta.y * sensitivity;
         
        if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
        if (m_Pitch < -89.0f) m_Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
    }

    glm::mat4 CameraComponent::getViewMatrix() const
    {
        return glm::lookAt(getOwner()->transform.position, getOwner()->transform.position + m_Front, m_Up);
    }

    glm::mat4 CameraComponent::getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(m_Fov),
            static_cast<float>(GWindow->getWidht()) / static_cast<float>(GWindow->getHeight()),
            0.1f, 
            100.0f); 
    }
}