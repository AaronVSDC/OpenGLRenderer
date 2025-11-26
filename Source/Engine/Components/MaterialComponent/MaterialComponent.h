#ifndef MATERIAL_COMPONENT_H
#define MATERIAL_COMPONENT_H

#include "OpenGLShader.h"
#include "BaseComponent.h"
#include "Texture.h" 
#include <glm/glm.hpp> 
#include <memory>
namespace Papyrus
{

    class MaterialComponent final : public BaseComponent
    {
    public: 

        MaterialComponent(std::unique_ptr<OpenGLShader> shader, std::unique_ptr<Texture> texture = nullptr); 
        ~MaterialComponent() = default;  
        MaterialComponent(const MaterialComponent&) = delete; 
        MaterialComponent(const MaterialComponent&&) = delete;
        MaterialComponent& operator=(const MaterialComponent&) = delete; 
        MaterialComponent& operator=(const MaterialComponent&&) = delete; 

        OpenGLShader* getShader() const { return m_pShader.get(); } 
        Texture* getTexture() const { return m_pTexture.get(); } 

    private: 
        std::unique_ptr<OpenGLShader> m_pShader = nullptr;
        std::unique_ptr<Texture> m_pTexture = nullptr;    
    };

}
#endif