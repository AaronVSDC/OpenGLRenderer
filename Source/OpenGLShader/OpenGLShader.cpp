#include "OpenGLShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>

namespace Papyrus 
{
    // Utility: read file to string
    static std::string ReadFile(const std::string& filepath)
    {
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (!in)
            std::cerr << "Failed to open file: " + filepath << std::endl;
        std::ostringstream contents;
        contents << in.rdbuf();
        return contents.str();
    } 
    //TODO: maybe consider seperating each shader type instead of creating 2 at once 
    //if we want to have a a geometry shader aswell or deferred rendering for example.
    OpenGLShader::OpenGLShader(const std::string& vertexPath,
        const std::string& fragmentPath)
    {
        std::string vertexSrc = ReadFile(vertexPath);
        std::string fragmentSrc = ReadFile(fragmentPath);

        m_RendererID = createProgram(vertexSrc, fragmentSrc); 
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID); 
    }

    uint32_t OpenGLShader::compileShader(uint32_t type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (!isCompiled)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::string infoLog(maxLength, '\0');
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            std::cerr << "Shader compile failure: " + infoLog << std::endl; 
        }

        return shader; 
    }

    uint32_t OpenGLShader::createProgram(const std::string& vertexSrc, const std::string& fragmentSrc) 
    {
        GLuint program = glCreateProgram();
        uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexSrc); 
        uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc); 

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program); 

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (!isLinked)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::string infoLog(maxLength, '\0');
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            glDeleteShader(vs);
            glDeleteShader(fs);

            throw std::runtime_error("Shader link failure: " + infoLog);
        }

        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::unbind() const
    {
        glUseProgram(0);
    }

    GLint OpenGLShader::getUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())  
            return m_UniformLocationCache[name]; 

        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }

    // Uniform upload helpers
    void OpenGLShader::uploadUniformInt(const std::string& name, int value)
    {
        GLint loc = getUniformLocation(name);
        glUniform1i(loc, value);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& name, float value)
    {
        GLint loc = getUniformLocation(name); 
        glUniform1f(loc, value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint loc = getUniformLocation(name);
        glUniform2f(loc, value.x, value.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint loc = getUniformLocation(name);
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint loc = getUniformLocation(name);
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint loc = getUniformLocation(name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint loc = getUniformLocation(name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}