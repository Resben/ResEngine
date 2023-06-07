/**
 * \file
 * \author Ben Hawkins
 * \author Christien Alden (34119981)
*/
#include "AEngine/Render/RenderCommandImpl.h"

namespace AEngine
{
		/**
		 * \class OpenGLRenderCommand
		 * \brief OpenGL implementation of RenderCommandImpl
		*/
	class OpenGLRenderCommand : public RenderCommandImpl
	{
	public:
		OpenGLRenderCommand() = default;
			/**
			 * \copydoc RenderCommandImpl::Clear
			*/
		void Clear() override;
			/**
			 * \copydoc RenderCommandImpl::SetClearColor
			*/
		void SetClearColor(const Math::vec4& color) override;
			/**
			 * \copydoc RenderCommandImpl::EnableDepthTest
			*/
		virtual void EnableDepthTest(bool set) override;
			/**
			 * \copydoc RenderCommandImpl::SetDepthTestFunction
			*/
		virtual void SetDepthTestFunction(RenderEnum function) override;
			/**
			 * \copydoc RenderCommandImpl::EnableBlend
			*/
		virtual void EnableBlend(bool set) override;
			/**
			 * \copydoc RenderCommandImpl::SetBlendFunction
			*/
		virtual void SetBlendFunction(RenderEnum source, RenderEnum destination) override;
			/**
			 * \copydoc RenderCommandImpl::SetViewport
			*/
		virtual void SetViewport(int x, int y, int width, int height) override;

			/**
			 * \copydoc RenderCommandImpl::PolygonMode
			*/
		virtual void PolygonMode(RenderEnum face, RenderEnum type) override;
			/**
			 * \copydoc RenderCommandImpl::DrawIndexed
			*/
		virtual void DrawIndexed(RenderEnum type, Size_t count, void* offset) override;
			/**
			 * \copydoc RenderCommandImpl::DrawArrays
			*/
		virtual void DrawArrays(RenderEnum type, int offset, Size_t count) override;

			/**
			 * \copydoc RenderCommandImpl::GetLibrary
			*/
		virtual RenderLibrary GetLibrary() override;
	};
}