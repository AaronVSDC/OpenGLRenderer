#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H
#include "BaseComponent.h"
#include "glad\glad.h"
#include <glm/glm.hpp>
#include <vector>  
#include <string> 

namespace Papyrus
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec2 uv;  
        glm::vec3 normal; 
    };

    struct Mesh
    {
        std::vector<Vertex> vertices; 
        std::vector<unsigned int> indices;
    };

    class MeshComponent : public BaseComponent
    {
    public:
        MeshComponent(const std::string& path) : m_FBXPath(path) {}

        void onEnable() override; 
        void onDisable() override;

        GLuint getVAO() const { return m_VAO;  } 
        GLuint getVBO() const { return m_VBO;  }
        GLuint getEBO() const { return m_EBO;  }
        unsigned int getIndexCount() const { return m_Mesh.indices.size(); }

    private:  
        Mesh loadFBX(const std::string& path); 

    private:  
        GLuint m_VAO, m_VBO, m_EBO;
        Mesh m_Mesh;
        std::string m_FBXPath;
    };
}
#endif