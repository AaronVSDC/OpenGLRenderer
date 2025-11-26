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
        //TODO: seperate user input from actual camera component

        Transform& transform = getOwner()->transform; 
        //------------------------
        //  KEYBOARD MOVEMENT
        //------------------------
        const bool* state = SDL_GetKeyboardState(nullptr);
        float speed = 2.5f * deltaTime;

        if (state[SDL_SCANCODE_W]) transform.position += speed * m_Front;
        if (state[SDL_SCANCODE_S]) transform.position -= speed * m_Front;
        if (state[SDL_SCANCODE_A]) transform.position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
        if (state[SDL_SCANCODE_D]) transform.position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;

        //------------------------
        //  MOUSE LOOK
        //------------------------
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_MOUSE_MOTION)
            {
                float xoff = e.motion.xrel * 0.1f;
                float yoff = e.motion.yrel * 0.1f;

                m_Yaw += xoff;
                m_Pitch -= yoff;

                if (m_Pitch > 89.0f) m_Pitch = 89.0f;
                if (m_Pitch < -89.0f) m_Pitch = -89.0f;

                glm::vec3 front;
                front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
                front.y = sin(glm::radians(m_Pitch));
                front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
                m_Front = glm::normalize(front);
            }

            if (e.type == SDL_EVENT_MOUSE_WHEEL)
            {
                m_Fov -= (float)e.wheel.y;
                if (m_Fov < 1.0f) m_Fov = 1.0f;
                if (m_Fov > 45.0f) m_Fov = 45.0f;
            }
        }
    }

    glm::mat4 CameraComponent::getViewMatrix() const
    {
        return glm::lookAt(getOwner()->transform.position, getOwner()->transform.position + m_Front, m_Up);
    }

    glm::mat4 CameraComponent::getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(m_Fov), static_cast<float>(GWindow->getWidht() / GWindow->getHeight()), 0.1f, 100.0f); 
    }
}