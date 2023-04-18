/**
 * @file
 * @author Christien Alden (34119981)
 * @brief OpenGLShader implementation
**/
#pragma once
#include "AEngine/Render/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace AEngine
{
	const class OpenGLShader : public Shader
	{
	public:
			/**
			 * @brief Loads shaders from file and compiles OpenGL shader
			 * @param[in] file containing shader code
			**/
		OpenGLShader(const std::string& ident, const std::string& fname);
		virtual ~OpenGLShader();

			/**
			 * @brief Bind shader to renderer
			 * @retval void
			**/
		void Bind() const override;
			/**
			 * @brief Unbind shader from renderer
			 * @retval void
			**/
		void Unbind() const override;

			/**
			 * @brief Upload single integer uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformInteger(const std::string& name, int value) const override;
			/**
			 * @brief Upload single float uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat(const std::string& name, float value) const override;
			/**
			 * @brief Upload a vec2 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat2(const std::string& name, const Math::vec2& value) const override;
			/**
			 * @brief Upload a vec3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat3(const std::string& name, const Math::vec3& value) const override;
			/**
			 * @brief Upload vec4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat4(const std::string& name, const Math::vec4& value) const override;
			/**
			 * @brief Upload a mat3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		void SetUniformMat3(const std::string& name, const Math::mat3& matrix) const override;
			/**
			 * @brief Upload a mat4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		void SetUniformMat4(const std::string& name, const Math::mat4& matrix) const override;

	private:
			// OpenGL object handle
		GLuint m_id = 0;

			/**
			 * @brief Compiles shader program from sources
			 * @param[in] sources all sources to be compiled
			 * @param[in] frag C-String of fragment shader source
			 * @return void
			**/
		void CompileProgram(std::unordered_map<GLenum, std::string> sources);
			/**
			 * Processes the source shader text into shader types
			 * @param source of shader text
			 * @returns a map of shader sources by type
			**/
		static std::unordered_map<GLenum, std::string> ProcessSource(const std::string& source);
			/**
			 * Converts between a string type and GLenum shader type
			 * @param type of shader
			 * @return GLenum
			**/
		static GLenum TypeToGLenum(const std::string& type);
			/**
			 * Converts between a GLenum and a string shader type
			 * @param type GLenum of shader
			 * @return string
			**/
		static std::string OpenGLShader::GLenumToType(GLenum type);

			/**
			 * @brief Compiles shader
			 * @param[in] shader source
			 * @return Unsigned int corresponding to OpenGL compiled shader
			**/
		static GLuint CompileShader(GLenum shaderType, const char* source);
			/**
			 * @brief Loads source from file
			 * @param[in] fname file containing shader source
			 * @return File contents as std::string
			**/
		static std::string LoadSource(const std::string& fname);
			/**
			 * @brief Checks shader compile status
			 * @param[in] type of shader to check
			 * @param[in] shaderId of shader to check
			 * @return void
			**/
		static void CheckShaderStatus(GLenum type, GLuint shaderId);
			/**
			 * @brief Checks shader program link status
			 * @param[in] programId to check
			 * @return void
			**/
		static void CheckProgramStatus(GLuint programId);
	};
}
