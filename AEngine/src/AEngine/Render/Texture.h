#pragma once
#include <string>
#include "AEngine/Core/Types.h"
#include "AEngine/Render/Types.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Math/Math.h"

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

			/**
			 * \brief Sets the wrap mode for horizontal axis
			 * \param[in] mode enum
			*/ 
		virtual void SetWrapS(TextureWrapMode mode) = 0;
			/**
			 * \brief Sets the wrap mode for vertical axis
			 * \param[in] mode enum
			*/ 
		virtual void SetWrapT(TextureWrapMode mode) = 0;
			/**
			 * \brief Sets the minification filter for the texture
			 * \param[in] filter enum
			*/ 
		virtual void SetMinFilter(TextureMinificationFilter filter) = 0;
			/**
			 * \brief Sets the magnification filter for the texture
			 * \param[in] filter enum
			*/ 
		virtual void SetMagnificationFilter(TextureMinificationFilter filter) = 0;
			/**
			 * \brief Sets the base mipmap level for the texture
			 * \param[in] baseLevel int
			*/ 
		virtual void SetTextureBaseLevel(int baseLevel) = 0;
			/**
			 * \brief Sets the maximum mipmap level for the texture
			 * \param[in] maxLevel int
			*/ 
		virtual void SetTextureMaxLevel(int maxLevel) = 0;
			/**
			 * \brief Sets the LOD bias for texture sampling
			 * \param[in] lodBias float
			*/ 
		virtual void SetTextureLODBias(float lodBias) = 0;
			/**
			 * \brief Sets the border color when CLAMP_TO_BORDER is used
			 * \param[in] borderColor Math::vec4 RGBA
			*/ 
		virtual void SetTextureBorderColor(Math::vec4 borderColor) = 0;

		static SharedPtr<Texture> Create(const std::string& ident, const std::string& fname);
	};
}
