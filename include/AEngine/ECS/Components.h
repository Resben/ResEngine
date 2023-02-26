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
#include "../Physics/Physics.h"

namespace AEngine
{

	class Mesh;
	class Shader;
	class Texture;
	class Font;

	struct TagComponent
	{
		std::string tag;
		uint32_t id;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag, uint32_t id)
			: tag(tag), id(id) {}
	};

	//--------------------------------------------------------------------------------
	// Modern Components
	//--------------------------------------------------------------------------------

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

	struct PhysicsComponent
	{
		ReactBody body;
	};

	struct ControllableComponent
	{
		void(*callback)(ReactBody&){ nullptr };
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

	struct CooldownComponent
	{
		float add;
		float time{ 0.0f };

		bool active() { return time >= 0.0; }
	};

	struct ScoreComponent
	{
		unsigned int score{ 0 };
		float timeRemaining{ 10.0f };
	};

	//--------------------------------------------------------------------------------
	// Legacy Components -- Can't change these
	//--------------------------------------------------------------------------------

	struct LegacyMeshComponent
	{
		unsigned int id{ 0 };
	};

	struct LegacyTextureComponent
	{
		Texture* texture{ nullptr };
	};

	struct LegacyTriggerComponent
	{
		glm::dvec3 min{ 0.0, 0.0, 0.0 };
		glm::dvec3 max{ 0.0, 0.0, 0.0 };
		bool* toggle{ nullptr };
	};

	struct FontComponent
	{
		Font* font{ nullptr };
		Shader* shader{ nullptr };
		std::string textToRender;
		glm::vec2 pos;
		float scale;
		glm::vec3 colour;
	};
}
