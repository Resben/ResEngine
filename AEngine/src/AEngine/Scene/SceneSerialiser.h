#pragma once
#include <yaml-cpp/yaml.h>
#include "Scene.h"

namespace AEngine
{
	class SceneSerialiser
	{
	public:
		static void DeserialiseFile(Scene* scene, const std::string& fname);
		static void DeserialiseNode(Scene* scene, YAML::Node node);

		static void SerialiseFile(Scene* scene, const std::string& fname);
		static YAML::Node SerialiseNode(Scene* scene);
		
	private:
		static void DeserialiseAsset(YAML::Node& root);
		static std::string DeserialiseTag(YAML::Node& root);
		static void DeserialiseTransform(YAML::Node& root, Entity& entity);
		static void DeserialiseRenderable(YAML::Node& root, Entity& entity);
		static void DeserialiseCamera(YAML::Node& root, Entity& entity);
		static void DeserialiseScript(YAML::Node& root, Entity& entity);
	};
}
