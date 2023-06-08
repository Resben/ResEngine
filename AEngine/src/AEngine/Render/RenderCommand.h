/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "Types.h"

namespace AEngine
{

	class RenderCommandImpl;

	class RenderCommand
	{
	public:
			/**
			 * \brief Initialize the render command
			 * \param[in] graphics The graphics library to use
			*/
		static void Initialise(RenderLibrary graphics);
			/**
			 * \brief Clear the current buffer
			*/
		static void Clear();
			/**
			 * \brief Set the clear color
			 * \param color The color to set
			*/
		static void SetClearColor(const Math::vec4& color);
			/**
			 * \brief Enable or disable the depth testing
			 * \param value True to enable, false to disable
			*/
		static void EnableDepthTest(bool value);
			/**
			 * \brief Set the depth test function
			 * \param function The function to set
			*/
		static void SetDepthTestFunction(DepthTestFunction function);
			/**
			 * \brief Enable or disable blending
			 * \param[in] value True to enable, false to disable
			*/
		static void EnableBlend(bool value);
			/**
			 * \brief Set the blend function
			 * \param[in] source The source function
			 * \param[in] destination The destination function
			*/
		static void SetBlendFunction(BlendFunction source, BlendFunction destination);
			/**
			 * \brief Set the blend constant
			 * \param[in] color The color to set
			 * \see BlendFunction::ConstantAlpha, BlendFunction::OneMinusConstantAlpha
			 * \see BlendFunction::ConstantColor, BlendFunction::OneMinusConstantColor
			*/
		static void SetBlendConstant(const Math::vec4& color);
			/**
			 * \brief Enable or disable face culling
			 * \param[in] value True to enable, false to disable
			*/
		static void EnableFaceCulling(bool value);
			/**
			 * \brief Set the cull face
			 * \param[in] face The face to cull
			*/
		static void SetCullFace(PolygonFace face);
			/**
			 * \brief Set the front face direction
			 * \param[in] direction The direction to set
			*/
		static void SetFrontFace(Winding direction);
			/**
			 * \brief Set the viewport
			 * \param[in] x viewport x position
			 * \param[in] y viewport y position
			 * \param[in] width viewport width
			 * \param[in] height viewport height
			*/
		static void SetViewport(int x, int y, int width, int height);

			/**
			 * \brief Set the polygon draw mode.
			 * \param[in] face to set the mode of.
			 * \param[in] type to set the mode to.
			*/
		static void PolygonMode(PolygonFace face, PolygonDraw type);
			/**
			 * \brief Draw an indexed array
			 * \param[in] type The type of primitive to draw
			 * \param[in] count The number of indices to draw
			 * \param[in] offset The offset in the index buffer
			*/
		static void DrawIndexed(PrimitiveDraw type, Intptr_t count, void* offset);
			/**
			 * \brief Draw an array
			 * \param[in] type The type of primitive to draw
			 * \param[in] offset The offset in the vertex buffer
			 * \param[in] count The number of vertices to draw
			*/
		static void DrawArrays(PrimitiveDraw type, int offset, Intptr_t count);

			/**
			 * \brief Get the current graphics library
			 * \return The current graphics library
			*/
		static RenderLibrary GetLibrary();

	private:
		static UniquePtr<RenderCommandImpl> s_impl;
	};
}
