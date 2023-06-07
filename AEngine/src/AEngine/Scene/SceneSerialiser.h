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
		static void SerialiseFile(Scene* scene, const std::string& fname);

		static void DeserialiseNode(Scene* scene, YAML::Node node);
		static YAML::Node SerialiseNode(Scene* scene);

	private:
		static void DeserialiseAsset(YAML::Node& root);
		static std::string DeserialiseTag(YAML::Node& root);
		static void DeserialiseTransform(YAML::Node& root, Entity& entity);
		static void DeserialiseRenderable(YAML::Node& root, Entity& entity);
		static void DeserialiseAnimation(YAML::Node& root, Entity& entity);
		static void DeserialiseTerrain(YAML::Node& root, Entity& entity);
		static void DeserialiseCamera(YAML::Node& root, Entity& entity);
		static void DeserialiseRigidBody(YAML::Node& root, Entity& entity);
		static void DeserialiseBoxCollider(YAML::Node& root, Entity& entity);
		static void DeserialiseScript(YAML::Node& root, Entity& entity);
		static void DeserialisePlayerController(YAML::Node& root, Entity& entity);
		static void DeserialiseSkybox(YAML::Node& root, Entity& entity);
	};
}
