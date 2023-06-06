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
	class GraphicsAPI;

	class RenderCommand
	{
	public:
			/**
			 * \brief Initialize the render command
			 * \param[in] graphics The graphics library to use
			*/
		static void Initialise(GraphicsLibrary graphics);
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
			 * \details
			 * Function:
			 * - DepthLessEqual: Passes if the incoming depth value is less than or equal to the stored depth value.
			 * - DepthGreaterEqual: Passes if the incoming depth value is greater than or equal to the stored depth value.
			 * - DepthLess: Passes if the incoming depth value is less than the stored depth value.
			 * - DepthGreater: Passes if the incoming depth value is greater than the stored depth value.
			 * - DepthEqual: Passes if the incoming depth value is equal to the stored depth value.
			*/
		static void SetDepthTestFunction(GraphicsEnum function);
			/**
			 * \brief Enable or disable blending
			 * \param[in] value True to enable, false to disable
			*/
		static void EnableBlend(bool value);
			/**
			 * \brief Set the blend function
			 * \param[in] source The source function
			 * \param[in] destination The destination function
			 * \details
			 * Source:
			 * - BlendZero: The source value is multiplied by zero.
			 * - BlendOne: The source value is multiplied by one.
			 * - BlendSourceColor: The source value is multiplied by the source color.
			 * - BlendOneMinusSourceColor: The source value is multiplied by one minus the source color.
			 * - BlendDestinationColor: The source value is multiplied by the destination color.
			 * - BlendOneMinusDestinationColor: The source value is multiplied by one minus the destination color.
			 * - BlendSourceAlpha: The source value is multiplied by the alpha value of the source color.
			 * - BlendOneMinusSourceAlpha: The source value is multiplied by one minus the alpha value of the source color.
			 * - BlendDestinationAlpha: The source value is multiplied by the alpha value of the destination color.
			 * - BlendOneMinusDestinationAlpha: The source value is multiplied by one minus the alpha value of the destination color.
			 * - BlendConstantColor: The source value is multiplied by a constant color.
			 * - BlendOneMinusConstantColor: The source value is multiplied by one minus a constant color.
			 * - BlendConstantAlpha: The source value is multiplied by a constant alpha value.
			 * - BlendOneMinusConstantAlpha: The source value is multiplied by one minus a constant alpha value.
			*/
		static void SetBlendFunction(GraphicsEnum source, GraphicsEnum destination);
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
			 * \details
			 * Face:
			 * - FaceFront: Front face of the polygon
			 * - FaceBack: Back face of the polygon
			 * - FaceFrontAndBack: Both front and back faces of the polygon
			 *
			 * Type:
			 * - PolygonFill: Fill the polygon
			 * - PolygonLine: Draw the polygon as lines
			*/
		static void PolygonMode(GraphicsEnum face, GraphicsEnum type);
			/**
			 * \brief Draw an indexed array
			 * \param[in] type The type of primitive to draw
			 * \param[in] count The number of indices to draw
			 * \param[in] offset The offset in the index buffer
			 * \details
			 * Type:
			 * - DrawPoints
			 * - DrawLines
			 * - DrawLineStrip
			 * - DrawLineLoop
			 * - DrawTriangle
			 * - DrawTriangleStrip
			 * - DrawTriangleFan
			 * - DrawQuads
			*/
		static void DrawIndexed(GraphicsEnum type, Size_t count, void* offset);
			/**
			 * \brief Draw an array
			 * \param[in] type The type of primitive to draw
			 * \param[in] offset The offset in the vertex buffer
			 * \param[in] count The number of vertices to draw
			 * \details
			 * Type:
			 * - DrawPoints
			 * - DrawLines
			 * - DrawLineStrip
			 * - DrawLineLoop
			 * - DrawTriangle
			 * - DrawTriangleStrip
			 * - DrawTriangleFan
			 * - DrawQuads
			*/
		static void DrawArrays(GraphicsEnum type, int offset, Size_t count);

			/**
			 * \brief Get the current graphics library
			 * \return The current graphics library
			*/
		static GraphicsLibrary GetLibrary();

	private:
		static UniquePtr<GraphicsAPI> s_impl;
	};
}
