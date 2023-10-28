/**
 * @file
 * @author Christien Alden (34119981)
 * @brief OpenGLShader implementation
 * @todo Implement AE asserts instead of exit(1)
**/
#include <fstream>
#include <iostream>
#include "AEngine/Core/Logger.h"
#include "OpenGLShader.h"

namespace AEngine
{
	OpenGLShader::OpenGLShader(const char *shaderSource)
		: Shader(shaderSource)
	{
		std::unordered_map<GLenum, std::string> sources = ProcessSource(shaderSource);
		CompileProgram(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& ident, const std::string& fname)
		: Shader(ident, fname)
	{
		AE_LOG_DEBUG("OpenGLShader::Constructor");
		std::string raw = LoadSource(fname);
		std::unordered_map<GLenum, std::string> sources = ProcessSource(raw);
		CompileProgram(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		AE_LOG_DEBUG("OpenGLShader::Destructor {}", this->GetIdent());
		glDeleteProgram(m_id);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_id);
	}

	void OpenGLShader::Unbind() const
	{
	  glUseProgram(0);
	}

	//--------------------------------------------------------------------------------
	// Uniforms
	//--------------------------------------------------------------------------------

	void OpenGLShader::SetUniformInteger(const std::string& name, int value) const
	{
		int32_t location = glGetUniformLocation(m_id, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) const
	{
	    int32_t location = glGetUniformLocation(m_id, name.c_str());
	    glUniform1f(location, value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const Math::vec2& value) const
	{
	    int32_t location = glGetUniformLocation(m_id, name.c_str());
	    glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const Math::vec3& value) const
	{
	    int32_t location = glGetUniformLocation(m_id, name.c_str());
	    glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const Math::vec4& value) const
	{
	    int32_t location = glGetUniformLocation(m_id, name.c_str());
	    glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const Math::mat3& matrix) const
	{
		int32_t location = glGetUniformLocation(m_id, name.c_str());
		glUniformMatrix3fv(location, 1, false, Math::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const Math::mat4& matrix) const
	{
	    int32_t location = glGetUniformLocation(m_id, name.c_str());
		glUniformMatrix4fv(location, 1, false, Math::value_ptr(matrix));
	}

	//--------------------------------------------------------------------------------
	// Private
	//--------------------------------------------------------------------------------

	void OpenGLShader::CompileProgram(std::unordered_map<GLenum, std::string> sources)
	{
		GLuint vertId = 0;
		GLuint fragId = 0;
		m_id = glCreateProgram();

		// for each in sources, compile program with correct GLenum
		auto it = sources.find(GL_VERTEX_SHADER);
		if (it != sources.end())
		{
			vertId = CompileShader(it->first, it->second.c_str());
			if (vertId != 0)
			{
				glAttachShader(m_id, vertId);
			}
		}

		it = sources.find(GL_FRAGMENT_SHADER);
		if (it != sources.end())
		{
			fragId = CompileShader(it->first, it->second.c_str());
			if (fragId != 0)
			{
				glAttachShader(m_id, fragId);
			}
		}

		glLinkProgram(m_id);
		CheckProgramStatus(m_id);

		glDeleteShader(vertId);
		glDeleteShader(fragId);
	}

	//--------------------------------------------------------------------------------
	// Static
	//--------------------------------------------------------------------------------

	std::string OpenGLShader::LoadSource(const std::string& fname)
	{
		std::ifstream infile(fname);
		if (!infile)
		{
			AE_LOG_ERROR("OpenGLShader::LoadSource::Error -> Could not open {}", fname);
			exit(1);
		}

		std::string source;
		infile.seekg(0, std::ios::end);
		Size_t size = infile.tellg();
		if (size == -1)
		{
			AE_LOG_TRACE("OpenGLShader::LoadSource::Error -> Invalid seek");
			exit(1);
		}

		source.resize(size);
		infile.seekg(0, std::ios::beg);
		infile.read(&source[0], size);

		return source;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ProcessSource(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* token = "#type";
		const Size_t tokenLen = strlen(token);

		Size_t pos = source.find(token, 0);
		while (pos != std::string::npos)
		{
			Size_t shaderTypeEol = source.find_first_of("\r\n", pos); // find end of token line
			Size_t shaderTypeBegin = pos + tokenLen + 1; // start of shader type
			std::string shaderType = source.substr(shaderTypeBegin, shaderTypeEol - shaderTypeBegin); // get type of shader

			Size_t shaderSourceBegin = source.find_first_not_of("\r\n", shaderTypeEol); // find start of the shader source
			pos = source.find(token, shaderSourceBegin); // find the first position of the next shader
			std::string shaderSource = source.substr(shaderSourceBegin, pos - shaderSourceBegin); // get shader source from substr

			// add to map
			shaderSources[TypeToGLenum(shaderType)] = shaderSource;
		}

		return shaderSources;
	}

	GLenum OpenGLShader::TypeToGLenum(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		AE_LOG_ERROR("OpenGLShader::TypeToGLenum -> Type not supported");
		exit(1);
	}

	std::string OpenGLShader::GLenumToType(GLenum type)
	{
		if (type == GL_VERTEX_SHADER)
			return "vertex";
		if (type == GL_FRAGMENT_SHADER)
			return "fragment";

		AE_LOG_ERROR("OpenGLShader::GLenumToType -> Type not supported");
		exit(1);
	}

	GLuint OpenGLShader::CompileShader(GLenum shaderType, const char* source)
	{
		GLuint shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &source, NULL);
		glCompileShader(shaderId);
		CheckShaderStatus(shaderType, shaderId);
		return shaderId;
	}

	void OpenGLShader::CheckShaderStatus(GLenum type, GLuint shaderId)
	{
		// check status
		GLint success = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint logSize = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
			char* infoLog = (char*) _malloca(logSize);

			glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);
			AE_LOG_FATAL("OpenGLShader::CheckShaderStatus::{}::Failed -> {}", GLenumToType(type), infoLog);
		}
	}

	void OpenGLShader::CheckProgramStatus(GLuint programId)
	{
		GLint success = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint logSize = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);
			char* infoLog = (char*) _malloca(logSize);

			glGetProgramInfoLog(programId, logSize, NULL, infoLog);
			AE_LOG_FATAL("OpenGLShader::CheckProgramStatus::Failed -> {}", infoLog);
		}
	}
}
