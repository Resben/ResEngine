#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "Types.h"

namespace AEngine
{
		/**
		 * \class RenderCommandImpl
		 * \brief Separates the internal implementation of a render command from the interface
		*/
	class RenderCommandImpl
	{
	public:
		virtual ~RenderCommandImpl() = default;
			/**
			 * \copydoc RenderCommand::Clear
			*/
		virtual void Clear() = 0;
			/**
			 * \copydoc RenderCommand::SetClearColor
			*/
		virtual void SetClearColor(const Math::vec4& color) = 0;
			/**
			 * \copydoc RenderCommand::EnableDepthTest
			*/
		virtual void EnableDepthTest(bool value) = 0;
			/**
			 * \copydoc RenderCommand::SetDepthTestFunction
			*/
		virtual void SetDepthTestFunction(DepthTestFunction function) = 0;
			/**
			 * \copydoc RenderCommand::EnableFaceCulling
			*/
		virtual void EnableFaceCulling(bool value) = 0;
			/**
			 * \copydoc RenderCommand::SetCullFace
			*/
		virtual void SetCullFace(PolygonFace face) = 0;
			/**
			 * \copydoc RenderCommand::SetFrontFace
			*/
		virtual void SetFrontFace(Winding direction) = 0;
			/**
			 * \copydoc RenderCommand::EnableBlend
			*/
		virtual void EnableBlend(bool value) = 0;
			/**
			 * \copydoc RenderCommand::SetBlendFunction
			*/
		virtual void SetBlendFunction(BlendFunction source, BlendFunction destination) = 0;
			/**
			 * \copydoc RenderCommand::SetViewport
			*/
		virtual void SetViewport(int x, int y, int width, int height) = 0;

			/**
			 * \copydoc RenderCommand::PolygonMode
			*/
		virtual void PolygonMode(PolygonFace face, PolygonDraw type) = 0;
			/**
			 * \copydoc RenderCommand::DrawIndexed
			*/
		virtual void DrawIndexed(PrimitiveDraw type, Intptr_t count, void* offset = 0) = 0;
			/**
			 * \copydoc RenderCommand::DrawArrays
			*/
		virtual void DrawArrays(PrimitiveDraw type, int offset, Intptr_t count) = 0;

			/**
			 * \copydoc RenderCommand::GetLibrary
			*/
		virtual RenderLibrary GetLibrary() = 0;
			/**
			 * \brief Create a new graphics API
			 * \param[in] graphics The graphics library to use
			 * \return The created graphics API
			 * \note This must be called before any other graphics API function
			*/
		static UniquePtr<RenderCommandImpl> Create(RenderLibrary graphics);
	};
}
