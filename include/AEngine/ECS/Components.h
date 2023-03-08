/**
 * @file
 * @brief Component supported by the ECS
 * @author Geoff Candy (34183006)
 * @author Christien Alden (34119981)
 **/
#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
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
		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation{ glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		const glm::mat4 ToMat4() const
		{
			return glm::translate(glm::mat4(1.0f), translation)
				* glm::toMat4(rotation)
				* glm::scale(glm::mat4(1.0f), scale);
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
		glm::vec3 colour{};
	};
}
