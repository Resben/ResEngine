/**
 * @file
 * @brief Component supported by the ECS
 * @author Geoff Candy (34183006)
 * @author Christien Alden (34119981)
 **/
#pragma once
#include <string>
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Physics/Collider.h"
#include "AEngine/Physics/CollisionBody.h"
#include "AEngine/Render/HeightMap.h"
#include "AEngine/Render/Model.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Script/EntityScript.h"

namespace AEngine
{
	class Mesh;
	class Shader;
	class Texture;
	class RigidBody;

	struct TagComponent
	{
		std::string tag;
		Uint16 ident;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag, Uint16 ident)
			: tag(tag), ident(ident) {}
	};

	struct TransformComponent
	{
		Math::vec3 translation{ 0.0f, 0.0f, 0.0f };
		Math::quat orientation{ Math::vec3(0.0f, 0.0f, 0.0f) };
		Math::vec3 scale{ 1.0f, 1.0f, 1.0f };

		const Math::mat4 ToMat4() const
		{
			return (
				Math::translate(Math::mat4(1.0f), translation)
				* Math::toMat4(orientation)
				* Math::scale(Math::mat4(1.0f), scale)
			);
		}
	};

	struct RenderableComponent
	{
		bool active;
		SharedPtr<Model> model;
		SharedPtr<Shader> shader;
	};

	struct TerrainComponent
	{
		bool active;
		SharedPtr<HeightMap> terrain;
		SharedPtr<Shader> shader;
		std::vector<std::string> textures;
		std::vector<Math::vec2> yRange;
	};

	struct CameraComponent
	{
		bool active;
		PerspectiveCamera camera;
	};

	struct ScriptableComponent
	{
		SharedPtr<EntityScript> script;
	};

	struct PhysicsHandle
	{
		CollisionBody *ptr = nullptr;
	};

	struct RigidBodyComponent
	{
		// runtime
		RigidBody *ptr = nullptr;

		// config
		float massKg;
		bool hasGravity;
	};

	struct BoxColliderComponent
	{
		// runtime
		Collider *ptr = nullptr;

		// config
		bool isTrigger;
		Math::vec3 size;
	};

	//struct SphereCollider
	//{
	//	// runtime
	//	Physics::Collider *collider;
	//	Physics::CollisionBody *body;

	//	// config
	//	bool isTrigger;
	//	float size;
	//	Math::vec3 center;
	//};
}
