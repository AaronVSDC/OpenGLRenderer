#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "glad\glad.h" 
namespace Papyrus
{
	struct TextureConfigInfo
	{
		int wrapS = GL_REPEAT;
		int wrapT = GL_REPEAT;
		int minFilter = GL_LINEAR_MIPMAP_LINEAR; 
		int magFilter = GL_LINEAR;
		int internalFormat = GL_RGB;
		int imageFormat = GL_RGB;
		bool generateMipmaps = true;
		bool flipVertically = true;
	};

	class Texture
	{

	public: 
		Texture(const std::string& filePath, TextureConfigInfo& configInfo = TextureConfigInfo()); 
		~Texture();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind(unsigned int unit) const;
		void unbind() const; 


		unsigned int getID() const { return m_TextureID;  }

	private: 

		unsigned int m_TextureID; 



	};



}



#endif