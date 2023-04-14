/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Shader class abstraction
 * @todo Implement AE asserts instead of exit(1)
**/
#include "../Core/Logger.h"

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Core/Math.hpp"
#include "Shader.h"

namespace AEngine
{

    Shader::Shader(const std::string& vertFile, const std::string& fragFile)
    {
        std::string vertShader;
        std::string fragShader;
        AE_LOG_TRACE("Shader::Vertex::Compile -> {}", vertFile);
        LoadSource(vertShader, vertFile);
        AE_LOG_TRACE("Shader::Fragment::Compile -> {}", fragFile);
        LoadSource(fragShader, fragFile);
        CompileProgram(vertShader.c_str(), fragShader.c_str());
    }

    Shader::~Shader()
    {
    	glDeleteProgram(m_id);
    }

    void Shader::Bind() const
    {
    	glUseProgram(m_id);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    //--------------------------------------------------------------------------------
    // Uniforms
    //--------------------------------------------------------------------------------

    void Shader::SetUniformFloat(const std::string& name, float value)
    {
        int32_t location = glGetUniformLocation(m_id, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::SetUniformFloat2(const std::string& name, const Math::vec2& value)
    {
        int32_t location = glGetUniformLocation(m_id, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void Shader::SetUniformFloat3(const std::string& name, const Math::vec3& value)
    {
        int32_t location = glGetUniformLocation(m_id, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::SetUniformFloat4(const std::string& name, const Math::vec4& value)
    {
        int32_t location = glGetUniformLocation(m_id, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniformMat3(const std::string& name, const Math::mat3& matrix)
    {
    	int32_t location = glGetUniformLocation(m_id, name.c_str());
    	glUniformMatrix3fv(location, 1, false, Math::value_ptr(matrix));
    }

    void Shader::SetUniformMat4(const std::string& name, const Math::mat4& matrix)
    {
        int32_t location = glGetUniformLocation(m_id, name.c_str());
    	glUniformMatrix4fv(location, 1, false, Math::value_ptr(matrix));
    }

    void Shader::SetUniformInteger(const std::string& name, int value)
    {
    	int32_t location = glGetUniformLocation(m_id, name.c_str());
    	glUniform1i(location, value);
    }

    //--------------------------------------------------------------------------------
    // Private
    //--------------------------------------------------------------------------------
    void Shader::CompileProgram(const char* vert, const char* frag)
    {
        unsigned int vertexShader = CompileVertexShader(vert);
        unsigned int fragmentShader = CompileFragmentShader(frag);

        // link shaders
        AE_LOG_TRACE("Shader::Program::Link");

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);
        glLinkProgram(m_id);

        CheckProgramStatus(m_id);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    //--------------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------------

    unsigned int Shader::CompileVertexShader(const char* source)
    {
        unsigned int shaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shaderId, 1, &source, NULL);
        glCompileShader(shaderId);
        CheckShaderStatus(shaderId, "Vertex");

        return shaderId;
    }

    unsigned int Shader::CompileFragmentShader(const char* source)
    {
        unsigned int shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shaderId, 1, &source, NULL);
        glCompileShader(shaderId);
        CheckShaderStatus(shaderId, "Fragment");

        return shaderId;
    }


    void Shader::LoadSource(std::string& shaderStr, const std::string& fname)
    {
    	std::stringstream ss;
    	std::ifstream fs(fname);
        if (!fs.is_open())
        {
            AE_LOG_ERROR("Shader::Source::Could_Not_Open_File -> {}", fname);
            exit(1);
        }

    	try
    	{
    		ss << fs.rdbuf();
    		fs.close();
    	}
    	catch (...)
    	{
            AE_LOG_ERROR("Shader::Source::Load_Failed -> {}", fname);
    		exit(1);
    	}

    	shaderStr = ss.str();
        if (shaderStr.size() == 0)
        {
            AE_LOG_ERROR("Shader::Source::Empty -> {}", fname);
            exit(1);
        }

        ss.clear();
    }

    void Shader::CheckShaderStatus(unsigned int shaderId, const char* type)
    {
    	// check status
    	GLint success = 0;
    	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    	if (!success)
    	{
    	    GLint logSize = 0;
    	    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    	    char* infoLog = (char*) alloca(logSize);

    	    glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
    	    AE_LOG_ERROR("Shader::{}::Compile::Failed -> {}", type, infoLog);
    	    exit(1);
    	}
    }

    void Shader::CheckProgramStatus(unsigned int programId)
    {
    	GLint success = 0;
    	glGetProgramiv(programId, GL_LINK_STATUS, &success);
    	if (!success)
    	{
    	    GLint logSize = 0;
    	    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
    	    char* infoLog = (char*) alloca(logSize);

    	    glGetProgramInfoLog(programId, logSize, NULL, infoLog);
    	    AE_LOG_ERROR("Shader::Program::Link::Failed -> {}", infoLog);
    	    exit(1);
    	}
    }
}
