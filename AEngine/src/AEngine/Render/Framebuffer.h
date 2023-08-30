#pragma once
#include "AEngine/Core/Types.h"
#include "Texture.h"
#include "Types.h"
#include <vector>

namespace AEngine
{
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = 0;

		virtual void SetActiveDrawBuffers(const std::vector<unsigned int>& buffers) = 0;
		virtual void OnWindowResize(Math::uvec2 windowSize) = 0;
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) = 0;
		virtual void Unbind() const = 0;

			/**
			 * \note \p index only applies to colorbuffer attachment
			*/
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) = 0;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) = 0;

		static SharedPtr<Framebuffer> Create(Math::uvec2 windowSize);

	protected:
		Framebuffer() = default;

	};
}
