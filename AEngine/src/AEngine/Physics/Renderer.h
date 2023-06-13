/**
 * \file   Renderer.h
 * \author Christien Alden (34119981)
*/

#pragma once
#include "AEngine/Math/Math.h"

namespace AEngine
{
	enum class PhysicsRendererItem
	{
		ColliderAABB,             ///< The AABB of the collider
		ColliderBroadphaseAABB,   ///< The AABB of the broadphase collider
		CollisionShape,           ///< The collision shape of the collider
		ContactPoint,             ///< The contact point of the collision
		ContactNormal             ///< The contact normal of the collision
	};

	enum class CollisionRenderShape
	{
		Box,            ///< Render Box collision shapes
		Sphere,         ///< Render Sphere collision shapes
		Capsule,        ///< Render Capsule collision shapes
		ConvexMesh,     ///< Render ConvexMesh collision shapes
		TriangleMesh,   ///< Render TriangleMesh collision shapes
		Heightfield     ///< Render Heightfield collision shapes
	};

	class PhysicsRenderer
	{
	public:
		virtual bool IsRenderItemEnabled(PhysicsRendererItem item) const = 0;
		virtual void SetRenderItem(PhysicsRendererItem item, bool enable) const = 0;
		virtual bool IsRenderShapeEnabled(CollisionRenderShape shape) const = 0;
		virtual void SetRenderShape(CollisionRenderShape shape, bool enable) const = 0;

	protected:
		virtual ~PhysicsRenderer() = default;
		virtual void Render(const Math::mat4& projectionView) const = 0;
	};
}
