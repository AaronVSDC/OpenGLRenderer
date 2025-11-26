#ifndef RENDERER_H
#define RENDERER_H
#include "Singleton.h"
#include <memory>
namespace Papyrus
{
	class OpenGLShader; 
	class Renderer final : public Singleton<Renderer> 
	{

	public:
		Renderer(); 
		~Renderer() override; 
		void render() const; 



	private: 
		std::unique_ptr<OpenGLShader> m_Shaders; 

	};




}




#endif