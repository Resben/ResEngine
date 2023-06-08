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
	class ReactPhysicsRenderer : public PhysicsRenderer
	{
	public:
		ReactPhysicsRenderer(rp3d::DebugRenderer& renderer);
		void Render(const Math::mat4& projectionView) const override;
		void SetRenderItem(PhysicsRendererItem item, bool enable) const override;
		bool IsRenderItemEnabled(PhysicsRendererItem item) const override;

	private:
		rp3d::DebugRenderer& m_renderer;
		SharedPtr<Shader> m_shader;
		SharedPtr<VertexArray> m_trianglesVertexArray;
		SharedPtr<VertexArray> m_linesVertexArray;

		friend class ReactPhysicsWorld;
	};
}