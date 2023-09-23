#pragma once
#include "AEngine/Core/Types.h"
#include "Texture.h"
#include "Types.h"
#include <vector>

namespace AEngine
{
		/**
		 * \class Framebuffer
		 * \brief Abstract framebuffer class
		 * \author Ben Hawkins (34112619) 
		**/
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = 0;

			/**
			 * \brief Transfer a depth buffer to another framebuffer
			 * \param[in] dest framebuffer ID to transfer to
			 * \retval void
			 * \todo Shouldn't rely on internal ID 
			**/
		virtual void TransferDepthBuffer(unsigned int dest) = 0;
			/**
			 * \brief Set the color attachment buffers to draw too
			 * \param[in] buffers Array of buffers to activate
			 * \retval void 
			**/
		virtual void SetActiveDrawBuffers(const std::vector<unsigned int>& buffers) = 0;
			/**
			 * \brief Resizes all buffers
			 * \param[in] size The new width and height
			 * \retval void 
			**/
		virtual void ResizeBuffers(Math::uvec2 size) = 0;
			/**
			 * \brief Select which buffers you want to bind as textures
			 * \param[in] buffers Array of buffers to bind
			 * \retval void 
			**/
		virtual void BindBuffers(const std::vector<unsigned int>& buffers) = 0;
			/**
			 * \brief Unbind the buffer textures 
			 * \retval void
			**/
		virtual void UnbindBuffers() = 0;
			/**
			 * \brief Bind the framebuffer
			 * \param[in] mode Set the framebuffer mode default: ReadWrite
			 * \retval void 
			**/
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) = 0;
			/**
			 * \brief Unbind the framebuffer
			 * \retval void 
			**/
		virtual void Unbind() = 0;
			/**
			 * \brief Attach a new buffer to the framebuffer
			 * \param[in] type The type of framebuffer attachment
			 * \param[in] index The color attachment index
			 * \note Index is only required for color attachments
			 * \retval void 
			**/
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) = 0;
			/**
			 * \brief Dettach a buffer from a framebuffer
			 * \param[in] type The type of framebuffer attachment
			 * \param[in] index The color attachment index
			 * \note Index is only required for color attachments
			 * \retval void 
			**/
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) = 0;

		static SharedPtr<Framebuffer> Create(Math::uvec2 windowSize);

	protected:
		Framebuffer() = default;

	};
}
