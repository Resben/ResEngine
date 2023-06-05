#pragma once
#include <string>
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"

namespace AEngine
{
		/**
		 * @class Texture
		 * @brief Abstract texture object
		 * @author Ben Hawkins
		 * @author Christien Alden (34119981)
		**/
	class Texture : public Asset
	{
	public:
		Texture(const std::string& ident, const std::string& path);
		virtual ~Texture() = default;

			/**
			 * @brief Binds the texture to the rendering API
			 * @return void
			**/
		virtual void Bind(unsigned int unit = 0) const = 0;
			/**
			 * @brief Unbinds the texture from the rendering API
			 * @return void
			**/
		virtual void Unbind() const = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		
		static SharedPtr<Texture> Create(const std::string& ident, const std::string& fname);
	};
}
