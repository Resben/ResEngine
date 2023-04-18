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

namespace AEngine
{
	class Mesh;
	class Shader;
	class Texture;

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

	struct CameraComponent
	{
		bool active;
		PerspectiveCamera camera;
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
