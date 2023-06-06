#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"

namespace AEngine
{
	enum class GraphicsLibrary
	{
		None = 0, OpenGL
	};

		/**
		 * \enum GraphicsEnum
		 * \brief Defines the available
		*/
	enum class GraphicsEnum
	{
		Invalid,
		DepthLessEqual, DepthGreaterEqual, DepthLess, DepthGreater, DepthEqual,
		FaceFront, FaceBack, FaceFrontAndBack,
		PolygonFill, PolygonLine,
		DrawTriangles, DrawTriangleStrip, DrawTriangleFan, DrawLines
	};

	class GraphicsAPI
	{
	public:
		GraphicsAPI() = default;
			/**
			 * \brief Clear the current buffer
			*/
		virtual void Clear() = 0;
			/**
			 * \brief Set the clear color
			 * \param color The color to set
			*/
		virtual void SetClearColor(const Math::vec4& color) = 0;
			/**
			 * \brief Enable or disable the depth testing
			 * \param value True to enable, false to disable
			*/
		virtual void EnableDepthTest(bool value) = 0;
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
		virtual void SetDepthTestFunction(GraphicsEnum function) = 0;
			/**
			 * \brief Set the viewport
			 * \param[in] x viewport x position
			 * \param[in] y viewport y position
			 * \param[in] width viewport width
			 * \param[in] height viewport height
			*/
		virtual void SetViewport(int x, int y, int width, int height) = 0;

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
		virtual void PolygonMode(GraphicsEnum face, GraphicsEnum type) = 0;
			/**
			 * \brief Draw an indexed array
			 * \param[in] type The type of primitive to draw
			 * \param[in] count The number of indices to draw
			 * \param[in] offset The offset in the index buffer
			 * \details
			 * Type:
			 * - DrawTriangle: Draw triangles
			 * - DrawTriangleStrip: Draw triangle strips
			 * - DrawTriangleFan: Draw triangle fans
			*/
		virtual void DrawIndexed(GraphicsEnum type, Size_t count, void* offset = 0) = 0;
			/**
			 * \brief Draw an array
			 * \param[in] type The type of primitive to draw
			 * \param[in] offset The offset in the vertex buffer
			 * \param[in] count The number of vertices to draw
			 * \details
			 * Type:
			 * - DrawTriangle: Draw triangles
			 * - DrawTriangleStrip: Draw triangle strips
			 * - DrawTriangleFan: Draw triangle fans
			*/
		virtual void DrawArrays(GraphicsEnum type, int offset, Size_t count) = 0;

			/**
			 * \brief Get the current graphics library
			 * \return The current graphics library
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
