#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "../Render/Shader.h"

namespace AEngine
{
		/**
		 * @class ShaderManager
		 * @brief Holds shared pointers to loaded Shaders
		 * @author Christien Alden (34119981)
		**/
	class ShaderManager
	{
	public:
			/**
			 * @retval ShaderManager* Instance of ShaderManager
			**/
		static ShaderManager* Instance();

			/**
			 * @brief Clears all loaded Shaders
			 * @retval void
			**/
		void Clear();

			/**
			 * @brief Loads a Shader into
			 * @param[in] name identifier to attach to Shader
			 * @param[in] vertexFilename filename of vertex shader
			 * @param[in] fragmentFilename filename of fragment shader
			 * @retval std::shared_ptr<Shader> to Shader just loaded
			**/
		std::shared_ptr<Shader> LoadShader(
			const std::string& name,
			const std::string& vertexFilename,
			const std::string& fragmentFilename
		);

			/**
			 * @brief Searches loaded shaders for a given Shader
			 * @param[in] shaderName to search for
			 * @retval Shader* if found
			 * @retval nullptr if not found
			**/
		Shader* GetShader(const std::string& shaderName);

		ShaderManager(const ShaderManager&) = delete;
		void operator=(const ShaderManager&) = delete;

	private:
		static ShaderManager* s_instance;
		ShaderManager() = default;

			/// Map of shader names to Shader objects
		std::map<std::string, std::shared_ptr<Shader>> m_shaders;
	};
}
