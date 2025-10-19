#ifndef OPENGLSHADER_H 
#define OPENGLSHADER_H
#include <string>
#include <unordered_map>
#include <glm\glm.hpp>
#include <glad\glad.h>
namespace Papyrus
{
    class OpenGLShader 
    {
    public:

        OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
        OpenGLShader(const OpenGLShader&) = delete; 
        OpenGLShader(const OpenGLShader&&) = delete; 
        OpenGLShader& operator=(const OpenGLShader&) = delete;  
        OpenGLShader& operator=(const OpenGLShader&&) = delete;  

        ~OpenGLShader();

        void bind() const;
        void unbind() const;

        void uploadUniformInt(const std::string& name, int value);
        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& value);
        void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        uint32_t compileShader(uint32_t type, const std::string& source);
        uint32_t createProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
        GLint getUniformLocation(const std::string& name);

        uint32_t m_RendererID = 0;
        std::unordered_map<std::string, GLint> m_UniformLocationCache;
    };

}

#endif