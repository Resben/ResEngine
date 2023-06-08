#pragma once
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Physics/CollisionBody.h"
#include "AEngine/Physics/Physics.h"
#include "ReactRenderer.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
	rp3d::Vector3 AEMathToRP3D(const Math::vec3& vec);

	rp3d::Quaternion AEMathToRP3D(const Math::quat& quat);

	Math::vec3 RP3DToAEMath(const rp3d::Vector3& vec);

	Math::quat RP3DToAEMath(const rp3d::Quaternion quat);

	class ReactPhysicsAPI : public PhysicsAPI
	{
	public:
		static ReactPhysicsAPI& Instance();
		virtual PhysicsWorld* CreateWorld(const PhysicsWorld::Props& props);
		virtual void DestroyWorld(PhysicsWorld* world);

		rp3d::PhysicsCommon* GetCommon();

	private:
		ReactPhysicsAPI();
		rp3d::PhysicsCommon m_common;
	};

	class ReactPhysicsWorld : public PhysicsWorld
	{
	public:
		ReactPhysicsWorld(rp3d::PhysicsCommon* common);
		virtual void Init(const Props& settings = Props()) override;
		virtual void OnUpdate(TimeStep deltaTime) override;
		virtual CollisionBody* AddCollisionBody(const Math::vec3& position, const Math::quat& orientation) override;
		virtual RigidBody* AddRigidBody(const Math::vec3& position, const Math::quat& orientation) override;

		virtual void Render(const Math::mat4& projectionView) const override;
		virtual bool IsRenderingEnabled() const override;
		virtual void SetRenderingEnabled(bool enable) override;
		virtual const ReactPhysicsRenderer* GetRenderer() const override;

		rp3d::PhysicsWorld* GetNative();
		float GetAccumulatorVal() const { return m_accumulator; }
		float GetUpdateStep() const { return m_updateStep; }

	private:
		rp3d::PhysicsWorld* m_world;
		UniquePtr<ReactPhysicsRenderer> m_renderer;
		TimeStep m_accumulator;
		TimeStep m_updateStep;
	};
}
