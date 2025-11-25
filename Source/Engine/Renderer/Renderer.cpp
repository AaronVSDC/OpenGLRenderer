#include "Renderer.h"
#include "SceneManager.h"

namespace Papyrus
{

	void Renderer::render() const
	{
		SceneManager::getInstance().render(); 

	}

}