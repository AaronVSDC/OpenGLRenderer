#include "Texture.h"
#include "glad\glad.h"
#include "stb_image.h"
#include <iostream>
namespace Papyrus
{
	Texture::Texture(const std::string& filepath, TextureConfigInfo& config)
	{

        glGenTextures(1, &m_TextureID); 
        glBindTexture(GL_TEXTURE_2D, m_TextureID); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter); 

        stbi_set_flip_vertically_on_load(config.flipVertically);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            if (nrChannels == 1)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                    GL_RED, GL_UNSIGNED_BYTE, data); 
            }
            else if (nrChannels == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, data); 
            }
            else if (nrChannels == 4)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, data);
            }

            if (config.generateMipmaps)
                glGenerateMipmap(GL_TEXTURE_2D); 
        }
        else
        {
            std::cerr << "Failed to load texture (ID " << m_TextureID << ") from: "
                << filepath << std::endl; 
        }
        stbi_image_free(data);

	}

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_TextureID); 
    }

    void Texture::bind(unsigned int unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}