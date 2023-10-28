#pragma once
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/Types.h"
#include "Scene.h"

namespace AEngine
{
	class SceneSerialiser
	{
	public:
		static UniquePtr<Scene> DeserialiseFile(const std::string& fname);
		static void DeserialiseNode(Scene* scene, YAML::Node node);

		static void SerialiseFile(Scene* scene, const std::string& fname);
		static YAML::Node SerialiseNode(Scene* scene);

	private:
		static Scene* s_scene;

		static YAML::Node SceneSerialiser::SerialiseVec4(glm::vec4& vec);
		static YAML::Node SceneSerialiser::SerialiseVec3(glm::vec3& vec);
		static YAML::Node SceneSerialiser::SerialiseVec2(glm::vec2& vec);

		static YAML::Node SerialiseColliders(CollisionBody* body);

		static std::string DeserialiseTag(YAML::Node& root);
		static void DeserialiseAsset(YAML::Node& root);
		static void DeserialiseTransform(YAML::Node& root, Entity& entity);
		static void DeserialiseRenderable(YAML::Node& root, Entity& entity);
		static void DeserialiseSkinnedRenderable(YAML::Node& root, Entity& entity);
		static void DeserialiseCamera(YAML::Node& root, Entity& entity);
		static void DeserialiseRigidBody(YAML::Node& root, Entity& entity);
		static void DeserialiseCollisionBody(YAML::Node& root, Entity& entity);
		static void DeserialiseCollider(YAML::Node& colliderNode, CollisionBody* body);
		static void DeserialiseScript(YAML::Node& root, Entity& entity);
		static void DeserialisePlayerController(YAML::Node& root, Entity& entity);
		static void DeserialiseSkybox(YAML::Node& root, Entity& entity);
		static void DeserialiseNavigationGridComponent(YAML::Node& root, Entity& entity);
		static void DeserialiseRectTransform(YAML::Node& root, Entity& entity);
		static void DeserialiseCanvasRenderer(YAML::Node& root, Entity& entity);
		static void DeserialiseText(YAML::Node& root, Entity& entity);
		static void DeserialiseImage(YAML::Node& root, Entity& entity);

	};
}
