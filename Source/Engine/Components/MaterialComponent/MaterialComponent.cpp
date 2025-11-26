#include "MaterialComponent.h"
 
namespace Papyrus 
{
	MaterialComponent::MaterialComponent(std::unique_ptr<OpenGLShader> shader, std::unique_ptr<Texture> texture)
		:m_pShader{std::move(shader)}, m_pTexture{std::move(texture)}
	{
	}
}