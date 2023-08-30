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

		virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) = 0;
		virtual void Unbind() const = 0;

			/**
			 * \note \p index only applies to colorbuffer attachment
			*/
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) = 0;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) = 0;

		static SharedPtr<Framebuffer> Create(unsigned int width, unsigned int height);

	protected:
		Framebuffer() = default;

	};
}
