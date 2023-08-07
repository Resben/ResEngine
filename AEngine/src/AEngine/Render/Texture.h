#pragma once
#include <string>
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Render/Types.h"
#include "AEngine/Resource/Asset.h"

namespace AEngine
{
		/**
		 * \class Texture
		 * \brief Abstract texture object
		 * \author Ben Hawkins
		 * \author Christien Alden (34119981)
		*/
	class Texture : public Asset
	{
	public:
			/**
			 * \param[in] ident The identifier for the Texture
			 * \param[in] path The path to the Texture
			*/
		Texture(const std::string& ident, const std::string& path);
		virtual ~Texture() = default;

			/**
			 * \brief Binds the texture to the rendering API
			**/
		virtual void Bind(unsigned int unit = 0) const = 0;
			/**
			 * \brief Unbinds the texture from the rendering API
			**/
		virtual void Unbind() const = 0;

			/**
			 * \brief Gets the width of the texture
			 * \return The width of the texture in pixels
			*/
		virtual int GetWidth() const = 0;
			/**
			 * \brief Gets the height of the texture
			 * \return The height of the texture in pixels
			*/
		virtual int GetHeight() const = 0;

			/**
			 * \brief Sets the wrap mode for horizontal axis
			 * \param[in] mode The wrap mode
			 *
			 * Defaults to TextureWrap::Repeat
			*/
		virtual void SetWrapS(TextureWrapMode mode) = 0;
			/**
			 * \brief Sets the wrap mode for vertical axis
			 * \param[in] mode The wrap mode
			 *
			 * Defaults to TextureWrap::Repeat
			*/
		virtual void SetWrapT(TextureWrapMode mode) = 0;
			/**
			 * \brief Sets the minification filter for the texture
			 * \param[in] filter The minimification filter
			 *
			 * Defaults to TextureFilter::Linear_Mipmap_Linear
			*/
		virtual void SetMinFilter(TextureFilter filter) = 0;
			/**
			 * \brief Sets the magnification filter for the texture
			 * \param[in] filter The magnification filter
			 *
			 * Defaults to TextureFilter::Linear
			*/
		virtual void SetMagnificationFilter(TextureFilter filter) = 0;
			/**
			 * \brief Sets the base mipmap level for the texture
			 * \param[in] baseLevel int
			 *
			 * Defaults to 0
			*/
		virtual void SetTextureBaseLevel(int baseLevel) = 0;
			/**
			 * \brief Sets the maximum mipmap level for the texture
			 * \param[in] maxLevel int
			 *
			 * Defaults to 1000
			*/
		virtual void SetTextureMaxLevel(int maxLevel) = 0;
			/**
			 * \brief Sets the LOD bias for texture sampling
			 * \param[in] lodBias float
			 *
			 * Defaults to 0.0f
			*/
		virtual void SetTextureLODBias(float lodBias) = 0;
			/**
			 * \brief Sets the border color when CLAMP_TO_BORDER is used
			 * \param[in] borderColor Math::vec4 RGBA
			 *
			 * Defaults to Math::vec4(0.0f, 0.0f, 0.0f, 0.0f)
			*/
		virtual void SetTextureBorderColor(Math::vec4 borderColor) = 0;

		static SharedPtr<Texture> Create(const std::string& ident, const std::string& fname);
	};
}
