#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "Types.h"

namespace AEngine
{
	class GraphicsAPI
	{
	public:
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
		virtual void SetDepthTestFunction(GraphicsEnum function) = 0;
			/**
			 * \copydoc RenderCommand::EnableBlend
			*/
		virtual void EnableBlend(bool value) = 0;
			/**
			 * \copydoc RenderCommand::SetBlendFunction
			*/
		virtual void SetBlendFunction(GraphicsEnum source, GraphicsEnum destination) = 0;
			/**
			 * \copydoc RenderCommand::SetViewport
			*/
		virtual void SetViewport(int x, int y, int width, int height) = 0;

			/**
			 * \copydoc RenderCommand::PolygonMode
			*/
		virtual void PolygonMode(GraphicsEnum face, GraphicsEnum type) = 0;
			/**
			 * \copydoc RenderCommand::DrawIndexed
			*/
		virtual void DrawIndexed(GraphicsEnum type, Size_t count, void* offset = 0) = 0;
			/**
			 * \copydoc RenderCommand::DrawArrays
			*/
		virtual void DrawArrays(GraphicsEnum type, int offset, Size_t count) = 0;

			/**
			 * \copydoc RenderCommand::GetLibrary
			*/
		virtual GraphicsLibrary GetLibrary() = 0;
			/**
			 * \brief Create a new graphics API
			 * \param[in] graphics The graphics library to use
			 * \return The created graphics API
			 * \note This must be called before any other graphics API function
			*/
		static UniquePtr<GraphicsAPI> Create(GraphicsLibrary graphics);
	};
}
