#include <fstream>
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/Identifier.h"
#include "AEngine/Core/Logger.h"
#include "Entity.h"
#include "SceneSerialiser.h"

/// @todo Remove managers
#include "AEngine/Resource/AssetManager.h"

//--------------------------------------------------------------------------------
// Custom Nodes
//--------------------------------------------------------------------------------
namespace YAML
{
	template<>
	struct convert<AEngine::Math::vec3> {
		static Node encode(const AEngine::Math::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, AEngine::Math::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

namespace AEngine
{
//--------------------------------------------------------------------------------
// File Serialisation
//--------------------------------------------------------------------------------
	void SceneSerialiser::DeserialiseFile(Scene* scene, const std::string& fname)
	{
		YAML::Node data = YAML::LoadFile(fname);
		if (!data)
		{
			AE_LOG_FATAL("Serialisation::LoadSceneFromFile::Failed -> No data");
		}

		// log scene
		std::string sceneName = data["scene"].as<std::string>();
		AE_LOG_INFO("Serialisation::LoadSceneFromFile::Start -> {} ({})", fname, sceneName);
		scene->m_ident = sceneName;

		DeserialiseNode(scene, data);
	}

	void SceneSerialiser::SerialiseFile(Scene* scene, const std::string& fname)
	{
		// generate node
		YAML::Node data = SerialiseNode(scene);

		// generate emitter
		YAML::Emitter em;
		em << data;

		// write to file
		std::ofstream fout(fname);
		fout << em.c_str();
		fout.close();
	}

//--------------------------------------------------------------------------------
// Node Serialisation
//--------------------------------------------------------------------------------
	YAML::Node SceneSerialiser::SerialiseNode(Scene* scene)
	{
		YAML::Node root;

		// scene ident
		root["scene"] = scene->m_ident;

		// populate assets
		YAML::Node assets;

		// models
		AssetManager<Model>& mm = AssetManager<Model>::Instance();
		std::map<std::string, std::shared_ptr<Model>>::const_iterator modItr;
		for (modItr = mm.begin(); modItr != mm.end(); ++modItr)
		{
			YAML::Node model;
			model["type"] = "model";
			model["path"] = modItr->second->GetPath();
			assets.push_back(model);
		}

		// shaders
		AssetManager<Shader>& sm = AssetManager<Shader>::Instance();
		std::map<std::string, std::shared_ptr<Shader>>::const_iterator sdrItr;
		for (sdrItr = sm.begin(); sdrItr != sm.end(); ++sdrItr)
		{
			YAML::Node shader;
			shader["type"] = "shader";
			shader["path"] = sdrItr->second->GetPath();
			assets.push_back(shader);
		}

		// textures
		AssetManager<Texture>& tm = AssetManager<Texture>::Instance();
		std::map<std::string, std::shared_ptr<Texture>>::const_iterator texItr;
		for (texItr = tm.begin(); texItr != tm.end(); ++texItr)
		{
			YAML::Node texture;
			texture["type"] = "texture";
			texture["path"] = texItr->second->GetPath();
			assets.push_back(texture);
		}

		// textures
		root["assets"] = assets;

		//populate entities
		YAML::Node entities;
		scene->m_Registry.each([&](auto entity) {
			YAML::Node entityNode;

			// Tag Component
			if (scene->m_Registry.all_of<TagComponent>(entity))
			{
				TagComponent& tag = scene->m_Registry.get<TagComponent>(entity);
				YAML::Node tagNode;
				tagNode["tag"] = tag.tag;
				entityNode["TagComponent"] = tagNode;
			}

			// Transform Component
			if (scene->m_Registry.all_of<TransformComponent>(entity))
			{
				// get data
				TransformComponent& transform = scene->m_Registry.get<TransformComponent>(entity);
				Math::vec3 translation = transform.translation;
				Math::vec3 rotation = Math::eulerAngles(transform.rotation);
				Math::vec3 scale = transform.scale;

				// convert rotation to degrees
				rotation.x = Math::degrees(rotation.x);
				rotation.y = Math::degrees(rotation.y);
				rotation.z = Math::degrees(rotation.z);

				// create node
				YAML::Node transformNode;
				transformNode["translation"] = translation;
				transformNode["rotation"] = rotation;
				transformNode["scale"] = scale;
				entityNode["TransformComponent"] = transformNode;
			}

			// Renderable Component
			if (scene->m_Registry.all_of<RenderableComponent>(entity))
			{
				// get data
				RenderableComponent& renderable = scene->m_Registry.get<RenderableComponent>(entity);
				bool isActive = renderable.active;
				std::string model = renderable.model->GetIdent();
				std::string shader = renderable.shader->GetIdent();

				// create node
				YAML::Node renderNode;
				renderNode["active"] = isActive;
				renderNode["model"] = model;
				renderNode["shader"] = shader;
				entityNode["RenderableComponent"] = renderNode;
			}

			// Camera Component
			if (scene->m_Registry.all_of<CameraComponent>(entity))
			{
				// get data
				CameraComponent& camera = scene->m_Registry.get<CameraComponent>(entity);
				bool isActive = camera.active;
				float fov = camera.camera.GetFov();
				float aspect = camera.camera.GetAspect();
				float nearPlane = camera.camera.GetNearPlane();
				float farPlane = camera.camera.GetFarPlane();

				// create camera sub-node
				YAML::Node camConfig;
				camConfig["fov"] = fov;
				camConfig["aspect"] = aspect;
				camConfig["nearPlane"] = nearPlane;
				camConfig["farPlane"] = farPlane;

				// create node
				YAML::Node cameraNode;
				cameraNode["active"] = isActive;
				cameraNode["camera"] = camConfig;
				entityNode["CameraComponent"] = cameraNode;
			}

			entities.push_back(entityNode);
		});

		root["entities"] = entities;
		return root;
	}

	void SceneSerialiser::DeserialiseNode(Scene* scene, YAML::Node data)
	{
		// assets
		YAML::Node assets = data["assets"];
		if (assets)
		{
			for (YAML::Node assetNode : assets)
			{
				SceneSerialiser::DeserialiseAsset(assetNode);
			}
		}

		// entities
		YAML::Node entities = data["entities"];
		if (entities)
		{
			for (YAML::Node entityNode : entities)
			{
				// generate tag for entity
				const std::string name = SceneSerialiser::DeserialiseTag(entityNode);
				Entity entity = scene->GetEntity(name);
				if (!entity)
				{
					// generte entity if doesn't exist
					entity = scene->CreateEntity(Identifier::Generate(), name);
				}

				SceneSerialiser::DeserialiseTransform(entityNode, entity);
				SceneSerialiser::DeserialiseRenderable(entityNode, entity);
				SceneSerialiser::DeserialiseCamera(entityNode, entity);
				SceneSerialiser::DeserialiseRigidBody(entityNode, entity);
				SceneSerialiser::DeserialiseBoxCollider(entityNode, entity);
			}
		}
	}

//--------------------------------------------------------------------------------
// Component Deserialisation
//--------------------------------------------------------------------------------
	inline void SceneSerialiser::DeserialiseAsset(YAML::Node& root)
	{
		std::string type = root["type"].as<std::string>();
		std::string path = root["path"].as<std::string>();

		if (type == "model")
		{
			AssetManager<Model>::Instance().Load(path);
		}
		else if (type == "shader")
		{
			AssetManager<Shader>::Instance().Load(path);
		}
		else if (type == "texture")
		{
			AssetManager<Texture>::Instance().Load(path);
		}
		else
		{
			AE_LOG_FATAL("Serialisation::Load::Asset::Failed -> Type '{}' doesn't exist", type);
		}
	}

	inline std::string SceneSerialiser::DeserialiseTag(YAML::Node& root)
	{
		std::string name;
		YAML::Node tagNode = root["TagComponent"];
		if (tagNode)
		{
			name = tagNode["tag"].as<std::string>();
		}

		return name;
	}

	inline void SceneSerialiser::DeserialiseTransform(YAML::Node& root, Entity& entity)
	{
		YAML::Node transformNode = root["TransformComponent"];
		if (transformNode)
		{
			// get data
			Math::vec3 translation = transformNode["translation"].as<Math::vec3>();
			Math::vec3 rotation = transformNode["rotation"].as<Math::vec3>();
			Math::vec3 scale = transformNode["scale"].as<Math::vec3>();

			// convert rotation to radians
			rotation.x = Math::radians(rotation.x);
			rotation.y = Math::radians(rotation.y);
			rotation.z = Math::radians(rotation.z);

			// set data
			TransformComponent* comp = entity.ReplaceComponent<TransformComponent>();
			comp->translation = translation;
			comp->rotation = rotation;
			comp->scale = scale;
		}
	}

	inline void SceneSerialiser::DeserialiseRenderable(YAML::Node& root, Entity& entity)
	{
		YAML::Node renderableNode = root["RenderableComponent"];
		if (renderableNode)
		{
			// get data
			bool active = renderableNode["active"].as<bool>();
			std::string model = renderableNode["model"].as<std::string>();
			std::string shader = renderableNode["shader"].as<std::string>();

			// set data
			RenderableComponent* comp = entity.ReplaceComponent<RenderableComponent>();
			comp->active = active;
			comp->model = AssetManager<Model>::Instance().Get(model);
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
		}
	}

	inline void SceneSerialiser::DeserialiseCamera(YAML::Node& root, Entity& entity)
	{
		YAML::Node cameraNode = root["CameraComponent"];
		if (cameraNode)
		{
			// get data
			bool active = cameraNode["active"].as<bool>();
			YAML::Node cameraSettings = cameraNode["camera"];
			float fov = cameraSettings["fov"].as<float>();
			float aspect = cameraSettings["aspect"].as<float>();
			float nearPlane = cameraSettings["nearPlane"].as<float>();
			float farPlane = cameraSettings["farPlane"].as<float>();

			// set data
			CameraComponent* comp = entity.ReplaceComponent<CameraComponent>();
			comp->active = active;
			comp->camera = PerspectiveCamera(fov, aspect, nearPlane, farPlane);
		}
	}

	inline void SceneSerialiser::DeserialiseRigidBody(YAML::Node& root, Entity& entity)
	{
		YAML::Node rigidBodyNode = root["RigidBodyComponent"];
		if (rigidBodyNode)
		{
			// get data
			float massKg = rigidBodyNode["mass"].as<float>();
			bool hasGravity = rigidBodyNode["gravity"].as<bool>();
			
			// set data
			RigidBodyComponent* comp = entity.ReplaceComponent<RigidBodyComponent>();
			comp->hasGravity = hasGravity;
			comp->massKg = massKg;
			comp->ptr = nullptr;
		}
	}

	inline void SceneSerialiser::DeserialiseBoxCollider(YAML::Node& root, Entity& entity)
	{
		YAML::Node boxColliderNode = root["BoxColliderComponent"];
		if (boxColliderNode)
		{
			// get data
			bool isTrigger = boxColliderNode["trigger"].as<bool>();
			Math::vec3 size = boxColliderNode["size"].as<Math::vec3>();

			// set data
			BoxColliderComponent* comp = entity.ReplaceComponent<BoxColliderComponent>();
			comp->isTrigger = isTrigger;
			comp->size= size;
			comp->ptr = nullptr;
		}
	}
}
