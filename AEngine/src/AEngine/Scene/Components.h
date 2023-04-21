/**
 * @file
 * @brief Component supported by the ECS
 * @author Geoff Candy (34183006)
 * @author Christien Alden (34119981)
 **/
#pragma once
#include <string>
#include "AEngine/Math/Math.hpp"
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Model.h"
#include "AEngine/Physics/CollisionBody.h"
#include "AEngine/Physics/Collider.h"
#include "AEngine/Script/Script.h"
#include "AEngine/Render/HeightMap.h"

namespace AEngine
{
	class Mesh;
	class Shader;
	class Texture;
	class RigidBody;

	struct TagComponent
	{
		std::string tag;
		uint16_t ident;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag, uint16_t ident)
			: tag(tag), ident(ident) {}
	};

	struct TransformComponent
	{
		Math::vec3 translation{ 0.0f, 0.0f, 0.0f };
		Math::quat rotation{ Math::vec3(0.0f, 0.0f, 0.0f) };
		Math::vec3 scale{ 1.0f, 1.0f, 1.0f };

		const Math::mat4 ToMat4() const
		{
			return (
				Math::translate(Math::mat4(1.0f), translation)
				* Math::toMat4(rotation)
				* Math::scale(Math::mat4(1.0f), scale)
			);
		}
	};

	struct RenderableComponent
	{
		bool active;
		std::shared_ptr<Model> model;
		std::shared_ptr<Shader> shader;
	};

	struct TerrainComponent
	{
		bool active;
		std::shared_ptr<HeightMap> terrain;
		std::shared_ptr<Shader> shader;
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
		std::shared_ptr<Script> script;
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
