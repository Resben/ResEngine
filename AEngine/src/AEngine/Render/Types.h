/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Contains the type definitions for the graphics API
*/
#pragma once

namespace AEngine
{
	enum class RenderLibrary
	{
		None = 0, OpenGL
	};

		/**
		 * \enum RenderEnum
		 * \brief Defines the available graphics flags
		*/
	enum class RenderEnum
	{
		// blend functions
		BlendZero, BlendOne,
		BlendSourceColor, BlendOneMinusSourceColor,
		BlendDestinationColor, BlendOneMinusDestinationColor,
		BlendSourceAlpha, BlendOneMinusSourceAlpha,
		BlendDestinationAlpha, BlendOneMinusDestinationAlpha,
		BlendConstantColor, BlendOneMinusConstantColor,
		BlendConstantAlpha, BlendOneMinusConstantAlpha,

		// depth test functions
		DepthLessEqual, DepthGreaterEqual, DepthLess, DepthGreater, DepthEqual,

		// polygon modes
		FaceFront, FaceBack, FaceFrontAndBack,

		// polygon draw modes
		PolygonFill, PolygonLine,

		// draw primitives
		DrawPoints,
		DrawLines, DrawLineStrip, DrawLineLoop,
		DrawTriangles, DrawTriangleStrip, DrawTriangleFan
	};

		/**
		 * \enum BufferUsage
		 * \brief Rendering API agnostic buffer usage
		 * \details
		 * The buffer usage is used to describe how the buffer will be used.
		*/
	enum class BufferUsage
	{
		StaticDraw, DynamicDraw, StreamDraw
	};

		/**
		 * \enum BufferElementType
		 * \brief Rendering API agnostic buffer element type
		 * \details
		 * The buffer element type is used to describe the type of the buffer element.
		*/
	enum class BufferElementType
	{
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4
	};
}
