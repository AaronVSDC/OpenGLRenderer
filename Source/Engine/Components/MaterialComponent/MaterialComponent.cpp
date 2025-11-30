#include "MaterialComponent.h"
 
namespace Papyrus 
{
	MaterialComponent::MaterialComponent(std::shared_ptr<OpenGLShader> shader, std::shared_ptr<Texture> texture)
		:m_pShader{std::move(shader)}, m_pTexture{std::move(texture)}
	{
	}
}