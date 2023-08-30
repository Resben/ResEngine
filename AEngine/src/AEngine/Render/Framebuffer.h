#pragma once
#include "AEngine/Core/Types.h"
#include "Types.h"

namespace AEngine
{
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = 0;

		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) const = 0;
		virtual void Unbind() const = 0;

			/**
			 * \note \p index only applies to colorbuffer attachment
			*/
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) = 0;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) = 0;

	protected:
		Framebuffer() = default;
		Texture m_colorBuffer;
		Texture m_depthBuffer;
		Texture m_stencilBuffer;

	};
}
