/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Contains the type definitions for the graphics API
*/
#pragma once

namespace AEngine
{
	enum class GraphicsLibrary
	{
		None = 0, OpenGL
	};

		/**
		 * \enum GraphicsEnum
		 * \brief Defines the available graphics flags
		*/
	enum class GraphicsEnum
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

	enum class BufferUsage
	{
		StaticDraw, DynamicDraw, StreamDraw
	};

	enum class BufferDataType
	{
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4
	};
}
