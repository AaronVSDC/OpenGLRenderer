#include "MeshComponent.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <iostream>
namespace Papyrus
{
	void MeshComponent::onEnable()
	{
        glEnable(GL_DEPTH_TEST); 

        auto& mesh = loadFBX(m_FBXPath);

        if (mesh.vertices.empty() || mesh.indices.empty()) 
        {
            std::cerr << "MeshComponent: no geometry loaded from '" << m_FBXPath << "'" << std::endl;
            return;
        }

        std::cout << "MeshComponent: loaded " << mesh.vertices.size() << " vertices and "
            << mesh.indices.size() << " indices from '" << m_FBXPath << "'" << std::endl;

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        // --- VBO ---
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER,
            mesh.vertices.size() * sizeof(Vertex), 
            mesh.vertices.data(),
            GL_STATIC_DRAW);

        // --- EBO ---
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh.indices.size() * sizeof(unsigned int),
            mesh.indices.data(),
            GL_STATIC_DRAW);

        // --- Vertex attributes ---

        // Position = layout(location = 0)
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, pos) 
        );
        glEnableVertexAttribArray(0); 

        // UV = layout(location = 1)
        glVertexAttribPointer( 
            1, 2, GL_FLOAT, GL_FALSE, 
            sizeof(Vertex), 
            (void*)offsetof(Vertex, uv) 
        );
        glEnableVertexAttribArray(1); 

        // Normal = layout(location = 2)
        glVertexAttribPointer( 
            2, 3, GL_FLOAT, GL_FALSE, 
            sizeof(Vertex), 
            (void*)offsetof(Vertex, normal) 
        );
        glEnableVertexAttribArray(2); 
         
        glBindVertexArray(0); 

	}
    void MeshComponent::onDisable()
    {
        if (m_EBO) glDeleteBuffers(1, &m_EBO);
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);

        m_EBO = m_VBO = m_VAO = 0; 
    }
	Mesh MeshComponent::loadFBX(const std::string& path)
	{
        m_Mesh.vertices.clear();
        m_Mesh.indices.clear();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_GenNormals |
            aiProcess_JoinIdenticalVertices |
            aiProcess_PreTransformVertices |
            aiProcess_FlipUVs
        );

        if (!scene || !scene->HasMeshes()) 
        {
            std::cerr << "Failed to load model: "
                << importer.GetErrorString() << std::endl;
            return m_Mesh;
        }

        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
        {
            const aiMesh* aMesh = scene->mMeshes[meshIndex];
            const unsigned int vertexOffset = static_cast<unsigned int>(m_Mesh.vertices.size());

            //-------------------------
            // LOAD VERTICES
            //-------------------------
            for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
            {
                Vertex v{};

                //Position
                v.pos = {
                    aMesh->mVertices[i].x,
                    aMesh->mVertices[i].y,
                    aMesh->mVertices[i].z
                };

                //UVs
                if (aMesh->HasTextureCoords(0)) {
                    v.uv = {
                        aMesh->mTextureCoords[0][i].x,
                        aMesh->mTextureCoords[0][i].y
                    };
                }
                else {
                    v.uv = { 0.0f, 0.0f };
                }

                //Normals
                if (aMesh->HasNormals()) {
                    v.normal = {
                        aMesh->mNormals[i].x,
                        aMesh->mNormals[i].y,
                        aMesh->mNormals[i].z
                    };
                }
                else {
                    v.normal = { 0,0,0 };
                }

                m_Mesh.vertices.push_back(v);
            }

            //----------------
            //LOAD INDICES
            //----------------
            for (unsigned int f = 0; f < aMesh->mNumFaces; f++)
            {
                const aiFace& face = aMesh->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++)
                {
                    m_Mesh.indices.push_back(vertexOffset + face.mIndices[i]);
                }
            }
        }

        return m_Mesh;
    }


}