/**
 * \file
 * \author Christien Alden (34119981)
*/

#pragma once
#include "AEngine/Physics/Renderer.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Render/VertexArray.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
		/**
		 * \class ReactPhysicsRenderer
		 * \brief Represents a renderer for ReactPhysics.
		 *
		 * This class inherits from PhysicsRenderer and provides rendering functionality for ReactPhysics simulations.
		 */
	class ReactPhysicsRenderer : public PhysicsRenderer
	{
	public:
			/**
			 * \brief Constructor for ReactPhysicsRenderer.
			 *
			 * \param renderer The rp3d::DebugRenderer to use for rendering.
			 */
		ReactPhysicsRenderer(rp3d::DebugRenderer& renderer);
			/**
			 * \brief Renders the physics objects using the specified projection-view matrix.
			 *
			 * \param projectionView The projection-view matrix.
			 */
		void Render(const Math::mat4& projectionView) const override;
			/**
			 * \brief Sets the rendering state of the specified PhysicsRendererItem.
			 *
			 * \param item The PhysicsRendererItem to enable or disable rendering for.
			 * \param enable True to enable rendering, false to disable rendering.
			 */
		void SetRenderItem(PhysicsRendererItem item, bool enable) const override;
			/**
			 * \brief Checks if rendering is enabled for the specified PhysicsRendererItem.
			 *
			 * \param item The PhysicsRendererItem to check.
			 * \return True if rendering is enabled, false otherwise.
			 */
		bool IsRenderItemEnabled(PhysicsRendererItem item) const override;
			/**
			 * \brief Gets the rendering state of the specified CollisionRenderShape.
			 * \param[in] shape The CollisionRenderShape to get the rendering state of.
			 * \retval True if rendering is enabled for the specified CollisionRenderShape.
			 * \retval False if rendering is disabled for the specified CollisionRenderShape.
			*/
		bool IsRenderShapeEnabled(CollisionRenderShape shape) const override;
			/**
			 * \brief Sets the rendering state of the specified CollisionRenderShape.
			 * \param[in] shape The CollisionRenderShape to set the rendering state of.
			 * \param[in] enable True to enable rendering, false to disable rendering.
			*/
		void SetRenderShape(CollisionRenderShape shape, bool enable) const override;

	private:
		rp3d::DebugRenderer& m_renderer;                 ///< The rp3d::DebugRenderer used for rendering.
		SharedPtr<Shader> m_shader;                      ///< The shader used for rendering.
		SharedPtr<VertexArray> m_trianglesVertexArray;   ///< The vertex array for rendering triangles.
		SharedPtr<VertexArray> m_linesVertexArray;       ///< The vertex array for rendering lines.
		unsigned int m_numLines;                         ///< The number of lines to render.
		unsigned int m_numTriangles;                     ///< The number of triangles to render.

			/**
			 * \brief Generates the render data for the physics objects.
			*/
		void GenerateRenderData();

		friend class ReactPhysicsWorld;
	};
}