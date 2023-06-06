/**
 * \file
 * \author Ben Hawkins
 * \author Christien Alden (34119981)
*/
#include "AEngine/Render/GraphicsAPI.h"

namespace AEngine
{
		/**
		 * \class OpenGLGraphicsAPI
		 * \brief OpenGL implementation of GraphicsAPI
		*/
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:
		OpenGLGraphicsAPI() = default;
			/**
			 * \copydoc GraphicsAPI::Clear
			*/
		void Clear() override;
			/**
			 * \copydoc GraphicsAPI::SetClearColor
			*/
		void SetClearColor(const Math::vec4& color) override;
			/**
			 * \copydoc GraphicsAPI::EnableDepthTest
			*/
		virtual void EnableDepthTest(bool set) override;
			/**
			 * \copydoc GraphicsAPI::SetDepthTestFunction
			*/
		virtual void SetDepthTestFunction(GraphicsEnum function) override;
			/**
			 * \copydoc GraphicsAPI::EnableBlend
			*/
		virtual void EnableBlend(bool set) override;
			/**
			 * \copydoc GraphicsAPI::SetBlendFunction
			*/
		virtual void SetBlendFunction(GraphicsEnum source, GraphicsEnum destination) override;
			/**
			 * \copydoc GraphicsAPI::SetViewport
			*/
		virtual void SetViewport(int x, int y, int width, int height) override;

			/**
			 * \copydoc GraphicsAPI::PolygonMode
			*/
		virtual void PolygonMode(GraphicsEnum face, GraphicsEnum type) override;
			/**
			 * \copydoc GraphicsAPI::DrawIndexed
			*/
		virtual void DrawIndexed(GraphicsEnum type, Size_t count, void* offset) override;
			/**
			 * \copydoc GraphicsAPI::DrawArrays
			*/
		virtual void DrawArrays(GraphicsEnum type, int offset, Size_t count) override;

			/**
			 * \copydoc GraphicsAPI::GetLibrary
			*/
		virtual RenderLibrary GetLibrary() override;
	};
}