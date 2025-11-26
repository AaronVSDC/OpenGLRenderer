#include "Transform.h"
#include <glm\ext\matrix_transform.hpp>

namespace Papyrus
{
    glm::mat4 Transform::getModelMatrix() 
    {
        glm::mat4 modelMatrix(1.0f);

        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationEuler.x), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationEuler.y), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationEuler.z), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, scale);

        return modelMatrix; 
    } 
}