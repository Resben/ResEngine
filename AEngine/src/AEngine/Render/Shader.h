/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Shader class inteface
**/
#pragma once
#include <glm/glm.hpp>
#include <string>
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/Asset.h"

namespace AEngine
{
	class Shader : public Asset
	{
	public:
		Shader(const std::string& ident, const std::string& path);
		virtual ~Shader() = default;

			/**
			 * @brief Bind shader to renderer
			 * @retval void
			**/
		virtual void Bind() const = 0;
			/**
			 * @brief Unbind shader from renderer
			 * @retval void
			**/
		virtual void Unbind() const = 0;
			/**
			 * @brief Upload single integer uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		virtual void SetUniformInteger(const std::string& name, int value) const = 0;
			/**
			 * @brief Upload single float uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		virtual void SetUniformFloat(const std::string& name, float value) const = 0;
			/**
			 * @brief Upload a vec2 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		virtual void SetUniformFloat2(const std::string& name, const Math::vec2& value) const = 0;
			/**
			 * @brief Upload a vec3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		virtual void SetUniformFloat3(const std::string& name, const Math::vec3& value) const = 0;
			/**
			 * @brief Upload vec4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] value to upload
			 * @retval void
			**/
		virtual void SetUniformFloat4(const std::string& name, const Math::vec4& value) const = 0;
			/**
			 * @brief Upload a mat3 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		virtual void SetUniformMat3(const std::string& name, const Math::mat3& matrix) const = 0;
			/**
			 * @brief Upload a mat4 uniform to shader
			 * @param[in] name of uniform
			 * @param[in] matrix to upload
			 * @retval void
			**/
		virtual void SetUniformMat4(const std::string& name, const Math::mat4& matrix) const = 0;
	
		static SharedPtr<Shader> Create(const std::string& ident, const std::string& fname);
	};
}
