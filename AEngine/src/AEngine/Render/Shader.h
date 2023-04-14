/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Shader class abstraction
**/
#pragma once
#include <string>
#include "../Math/Math.hpp"

namespace AEngine
{
	class Shader
	{
	public:
			/**
			 * @brief Loads shaders from file and compiles OpenGL shader
			 * @param[in] vertFile file containing vertex shader
			 * @param[in] fragFile file containing fragment shader
			**/
		Shader(const std::string& vertFile, const std::string& fragFile);
		~Shader();

			/**
			 * @brief Bind shader to renderer
			 * @retval void
			**/
		void Bind() const;
			/**
			 * @brief Unbind shader from renderer
			 * @retval void
			**/
		void Unbind() const;

			/**
			 * @brief Upload single integer uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformInteger(const std::string& name, int value);
			/**
			 * @brief Upload single float uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat(const std::string& name, float value);
			/**
			 * @brief Upload a vec2 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat2(const std::string& name, const Math::vec2& value);
			/**
			 * @brief Upload a vec3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat3(const std::string& name, const Math::vec3& value);
			/**
			 * @brief Upload vec4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		void SetUniformFloat4(const std::string& name, const Math::vec4& value);
			/**
			 * @brief Upload a mat3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		void SetUniformMat3(const std::string& name, const Math::mat3& matrix);
			/**
			 * @brief Upload a mat4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		void SetUniformMat4(const std::string& name, const Math::mat4& matrix);

	private:
			/// OpenGL object handle to shader
		unsigned int m_id{0};
			/**
			 * @brief Compiles shader program from sources
			 * @param[in] vert C-String of vertex shader source
			 * @param[in] frag C-String of fragment shader source
			 * @return void
			**/
		void CompileProgram(const char* vert, const char* frag);

			/**
			 * @brief Compiles vertex shader
			 * @param[in] source Vertex shader source
			 * @return Unsigned int corresponding to OpenGL compiled shader
			**/
		static unsigned int CompileVertexShader(const char* source);
			/**
			 * @brief Compiles fragment shader
			 * @param[in] source fragment shader source
			 * @return Unsigned int corresponding to OpenGL compiled shader
			**/
		static unsigned int CompileFragmentShader(const char* source);
			/**
			 * @brief Loads source from file
			 * @param[out] shaderStr to store the shader source
			 * @param[in] fname file containing shader source
			 * @retval void
			**/
		static void LoadSource(std::string& shaderStr, const std::string& fname);
			/**
			 * @brief Checks shader compile status
			 * @param[in] shaderId of shader to check
			 * @param[in] type of shader; modifies error message
			 * @return void
			**/
		static void CheckShaderStatus(unsigned int shaderId, const char* type);
			/**
			 * @brief Checks shader program link status
			 * @param[in] programId to check
			 * @return void
			**/
		static void CheckProgramStatus(unsigned int programId);
	};
}
