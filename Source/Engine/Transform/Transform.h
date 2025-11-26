#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>

namespace Papyrus
{
	struct Transform final
	{
		glm::vec3 position{};
		glm::vec3 rotationEuler{ 0.0f };
		glm::vec3 scale{ 1.f }; 

		glm::mat4 getModelMatrix(); 
	};
}
#endif