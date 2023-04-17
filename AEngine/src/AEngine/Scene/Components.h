/**
 * @file
 * @brief Component supported by the ECS
 * @author Geoff Candy (34183006)
 * @author Christien Alden (34119981)
 **/
#pragma once
#include <string>
#include "../Math/Math.hpp"
#include "../Core/PerspectiveCamera.h"

namespace AEngine
{
	class Mesh;
	class Shader;
	class Texture;

	struct TagComponent
	{
		std::string tag;
		uint32_t id;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag, uint32_t id)
			: tag(tag), id(id) {}
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
		bool active{ true };
		Mesh* mesh{ nullptr };
		Texture* texture{ nullptr };
		Shader* shader{ nullptr };
	};

	struct CameraComponent
	{
		bool active{ false };
		PerspectiveCamera cam;
	};

	struct LightComponent
	{
		Math::vec3 colour{};
	};

	// not to be used by the client...
	//struct PhysicsHandle
	//{
	//	Physics::CollisionBody *ptr;
	//};

	//struct RigidBodyComponent
	//{
	//	Physics::RigidBody *ptr;
	//	float massKg;
	//	bool hasGravity;
	//};

	//struct BoxCollider
	//{
	//	// runtime
	//	Physics::Collider *collider;
	//	Physics::CollisionBody *body;

	//	// config
	//	bool isTrigger;
	//	Math::vec3 size;
	//	Math::vec3 center;
	//};

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
