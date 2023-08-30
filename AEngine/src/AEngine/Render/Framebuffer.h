#pragma once
#include "AEngine/Core/Types.h"
#include "Texture.h"
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

		static SharedPtr<Framebuffer> Create();

	protected:
		Framebuffer() = default;
		SharedPtr<Texture> m_colorBuffer;
		SharedPtr<Texture> m_depthBuffer;
		SharedPtr<Texture> m_stencilBuffer;

	};
}
