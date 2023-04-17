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
	};
}
