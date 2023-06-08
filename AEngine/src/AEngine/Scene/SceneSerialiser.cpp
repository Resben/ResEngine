#include <fstream>
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/Identifier.h"
#include "AEngine/Core/Logger.h"
#include "Entity.h"
#include "SceneSerialiser.h"
#include "AEngine/Script/ScriptEngine.h"
#include "AEngine/Skybox/Skybox.h"
#include "AEngine/Water/Water.h"

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

	template<>
	struct convert<AEngine::Math::vec2> {
		static Node encode(const AEngine::Math::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, AEngine::Math::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
}

namespace AEngine
{
//--------------------------------------------------------------------------------
// File Serialisation
//--------------------------------------------------------------------------------
	UniquePtr<Scene> SceneSerialiser::DeserialiseFile(const std::string& fname)
	{
		YAML::Node data = YAML::LoadFile(fname);
		if (!data)
		{
			AE_LOG_ERROR("Serialisation::LoadSceneFromFile::Failed -> No data");
			return nullptr;
		}

		// strip file name
		Size_t last = fname.find_last_of("/");
		const std::string ident = fname.substr(last + 1);
		Size_t ext = ident.find_first_of(".");
		const std::string sceneName = ident.substr(0, ext);

		// log and create scene
		AE_LOG_INFO("Serialisation::LoadSceneFromFile::Start -> {}", sceneName);
		UniquePtr<Scene> scene(new Scene(sceneName));

		DeserialiseNode(scene.get(), data);
		return scene;
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

		// populate assets
		YAML::Node assets;

		// models
		AssetManager<Model>& mm = AssetManager<Model>::Instance();
		std::map<std::string, SharedPtr<Model>>::const_iterator modItr;
		for (modItr = mm.begin(); modItr != mm.end(); ++modItr)
		{
			YAML::Node model;
			model["type"] = "model";
			model["path"] = modItr->second->GetPath();
			assets.push_back(model);
		}

		// animation
		AssetManager<Animation>& aa = AssetManager<Animation>::Instance();
		std::map<std::string, SharedPtr<Animation>>::const_iterator aaItr;
		for (aaItr = aa.begin(); aaItr != aa.end(); ++aaItr)
		{
			YAML::Node anim;
			anim["type"] = "animation";
			anim["path"] = aaItr->second->GetPath();
			assets.push_back(anim);
		}

		// terrain
		AssetManager<HeightMap>& tem = AssetManager<HeightMap>::Instance();
		std::map<std::string, SharedPtr<HeightMap>>::const_iterator terItr;
		for (terItr = tem.begin(); terItr != tem.end(); ++terItr)
		{
			YAML::Node terrain;
			terrain["type"] = "map";
			terrain["path"] = terItr->second->GetPath();
			assets.push_back(terrain);
		}

		// shaders
		AssetManager<Shader>& sm = AssetManager<Shader>::Instance();
		std::map<std::string, SharedPtr<Shader>>::const_iterator sdrItr;
		for (sdrItr = sm.begin(); sdrItr != sm.end(); ++sdrItr)
		{
			YAML::Node shader;
			shader["type"] = "shader";
			shader["path"] = sdrItr->second->GetPath();
			assets.push_back(shader);
		}

		// textures
		AssetManager<Texture>& tm = AssetManager<Texture>::Instance();
		std::map<std::string, SharedPtr<Texture>>::const_iterator texItr;
		for (texItr = tm.begin(); texItr != tm.end(); ++texItr)
		{
			YAML::Node texture;
			texture["type"] = "texture";
			texture["path"] = texItr->second->GetPath();
			assets.push_back(texture);
		}

		// scripts
		AssetManager<Script>& scrm = AssetManager<Script>::Instance();
		std::map<std::string, SharedPtr<Script>>::const_iterator scrItr;
		for (scrItr = scrm.begin(); scrItr != scrm.end(); ++scrItr)
		{
			YAML::Node script;
			script["type"] = "script";
			script["path"] = scrItr->second->GetPath();
			assets.push_back(script);
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
				Math::vec3 orientation = Math::eulerAngles(transform.orientation);
				Math::vec3 scale = transform.scale;

				// convert orientation to degrees
				orientation.x = Math::degrees(orientation.x);
				orientation.y = Math::degrees(orientation.y);
				orientation.z = Math::degrees(orientation.z);

				// create node
				YAML::Node transformNode;
				transformNode["translation"] = translation;
				transformNode["orientation"] = orientation;
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

			// Renderable Component
			if (scene->m_Registry.all_of<AnimationComponent>(entity))
			{
				// get data
				AnimationComponent& animate = scene->m_Registry.get<AnimationComponent>(entity);
				bool isActive = animate.active;
				std::string model = animate.model->GetIdent();
				std::string shader = animate.shader->GetIdent();
				std::string animation = animate.animator.GetName();

				// create node
				YAML::Node animateNode;
				animateNode["active"] = isActive;
				animateNode["model"] = model;
				animateNode["shader"] = shader;
				animateNode["startAnimation"] = animation;

				entityNode["AnimationComponent"] = animateNode;
			}

			// Terrain Component
			if (scene->m_Registry.all_of<TerrainComponent>(entity))
			{
				// get data
				TerrainComponent& terrain = scene->m_Registry.get<TerrainComponent>(entity);
				bool isActive = terrain.active;
				std::string model = terrain.terrain->GetIdent();
				std::string shader = terrain.shader->GetIdent();

				// create node
				YAML::Node terrainNode;
				terrainNode["active"] = isActive;
				terrainNode["terrain"] = model;
				terrainNode["shader"] = shader;

				YAML::Node texturesNode;
				for (unsigned int i = 0; i < terrain.textures.size(); i++)
				{
					YAML::Node textureNode;
					textureNode["texture"] = terrain.textures[i];
					textureNode["range"] = terrain.yRange[i];
					texturesNode.push_back(textureNode);
				}

				terrainNode["textures"] = texturesNode;

				entityNode["TerrainComponent"] = terrainNode;
			}

			// Camera Component
			if (scene->m_Registry.all_of<CameraComponent>(entity))
			{
				// get data
				CameraComponent& camera = scene->m_Registry.get<CameraComponent>(entity);
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
				cameraNode["camera"] = camConfig;
				entityNode["CameraComponent"] = cameraNode;
			}

			// Scriptable Component
			if (scene->m_Registry.all_of<ScriptableComponent>(entity))
			{
				// get data
				ScriptableComponent& script = scene->m_Registry.get<ScriptableComponent>(entity);
				YAML::Node scriptNode;
				scriptNode["script"] = script.script->GetIdent();
				entityNode["ScriptableComponent"] = scriptNode;
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
					entity = scene->CreateEntity(name);
				}

				SceneSerialiser::DeserialiseTransform(entityNode, entity);
				SceneSerialiser::DeserialiseRenderable(entityNode, entity);
				SceneSerialiser::DeserialiseAnimation(entityNode, entity);
				SceneSerialiser::DeserialiseTerrain(entityNode, entity);
				SceneSerialiser::DeserialiseCamera(entityNode, entity);
				SceneSerialiser::DeserialiseRigidBody(entityNode, entity);
				SceneSerialiser::DeserialiseBoxCollider(entityNode, entity);
				SceneSerialiser::DeserialiseScript(entityNode, entity);
				SceneSerialiser::DeserialisePlayerController(entityNode, entity);
				SceneSerialiser::DeserialiseSkybox(entityNode, entity);
				SceneSerialiser::DeserialiseWater(entityNode, entity);
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
		else if (type == "map")
		{
			AssetManager<HeightMap>::Instance().Load(path);
		}
		else if (type == "shader")
		{
			AssetManager<Shader>::Instance().Load(path);
		}
		else if (type == "texture")
		{
			AssetManager<Texture>::Instance().Load(path);
		}
		else if (type == "script")
		{
			AssetManager<Script>::Instance().Load(path);
		}
		else if (type == "model")
		{
			AssetManager<Model>::Instance().Load(path);
		}
		else if (type == "animation")
		{
			AssetManager<Animation>::Instance().Load(path);
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
			Math::vec3 orientation = transformNode["orientation"].as<Math::vec3>();
			Math::vec3 scale = transformNode["scale"].as<Math::vec3>();

			// convert orientation to radians
			orientation.x = Math::radians(orientation.x);
			orientation.y = Math::radians(orientation.y);
			orientation.z = Math::radians(orientation.z);

			// set data
			TransformComponent* comp = entity.ReplaceComponent<TransformComponent>();
			comp->translation = translation;
			comp->orientation = orientation;
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

	inline void SceneSerialiser::DeserialiseAnimation(YAML::Node& root, Entity& entity)
	{
		YAML::Node animateNode = root["AnimationComponent"];
		if (animateNode)
		{
			// get data
			bool active = animateNode["active"].as<bool>();
			std::string model = animateNode["model"].as<std::string>();
			std::string shader = animateNode["shader"].as<std::string>();
			std::string animation = animateNode["startAnimation"].as<std::string>();

			// set data
			AnimationComponent* comp = entity.ReplaceComponent<AnimationComponent>();

			comp->active = active;
			comp->model = AssetManager<Model>::Instance().Get(model);
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
			comp->animator.Load(*AssetManager<Animation>::Instance().Get(animation));
		}
	}

	inline void SceneSerialiser::DeserialiseTerrain(YAML::Node& root, Entity& entity)
	{
		YAML::Node terrainNode = root["TerrainComponent"];
		if (terrainNode)
		{
			// get data
			bool active = terrainNode["active"].as<bool>();
			std::string terrain = terrainNode["terrain"].as<std::string>();
			std::string shader = terrainNode["shader"].as<std::string>();

			// set data
			TerrainComponent* comp = entity.ReplaceComponent<TerrainComponent>();

			for (const auto& textureNode : terrainNode["textures"])
			{
				comp->textures.push_back(textureNode["texture"].as<std::string>());
				comp->yRange.push_back(textureNode["range"].as<Math::vec2>());
			}

			comp->active = active;
			comp->terrain = AssetManager<HeightMap>::Instance().Get(terrain);
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
		}
	}

	inline void SceneSerialiser::DeserialiseCamera(YAML::Node& root, Entity& entity)
	{
		YAML::Node cameraNode = root["CameraComponent"];
		if (cameraNode)
		{
			// get data
			YAML::Node cameraSettings = cameraNode["camera"];
			float fov = cameraSettings["fov"].as<float>();
			float aspect = cameraSettings["aspect"].as<float>();
			float nearPlane = cameraSettings["nearPlane"].as<float>();
			float farPlane = cameraSettings["farPlane"].as<float>();

			// set data
			CameraComponent* comp = entity.ReplaceComponent<CameraComponent>();
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
			std::string strType = rigidBodyNode["type"].as<std::string>();
			RigidBody::Type type;

			if (strType == "dynamic")
			{
				type = RigidBody::Type::DYNAMIC;
			}
			else if (strType == "kinematic")
			{
				type = RigidBody::Type::KINEMATIC;
			}
			else if (strType == "static")
			{
				type = RigidBody::Type::STATIC;
			}
			else
			{
				AE_LOG_FATAL("Serialisation::DeserialiseRigidBody::Failed -> Type '{}' doesn't exist", strType);
			}

			// set data
			RigidBodyComponent* comp = entity.ReplaceComponent<RigidBodyComponent>();
			comp->hasGravity = hasGravity;
			comp->massKg = massKg;
			comp->type = type;
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

	inline void SceneSerialiser::DeserialiseScript(YAML::Node& root, Entity& entity)
	{
		YAML::Node scriptNode = root["ScriptableComponent"];
		if (scriptNode)
		{
			std::string scriptIdent = scriptNode["script"].as<std::string>();
			ScriptableComponent* comp = entity.ReplaceComponent<ScriptableComponent>();
			Script* script = AssetManager<Script>::Instance().Get(scriptIdent).get();
			comp->script = MakeUnique<EntityScript>(entity, ScriptEngine::GetState(), script);
		}
	}

	inline void SceneSerialiser::DeserialisePlayerController(YAML::Node& root, Entity& entity)
	{
		YAML::Node playerControllerNode = root["PlayerControllerComponent"];
		if (playerControllerNode)
		{
			// get data
			float radius = playerControllerNode["radius"].as<float>();
			float height = playerControllerNode["height"].as<float>();
			float speed = playerControllerNode["speed"].as<float>();
			float moveDrag = playerControllerNode["moveDrag"].as<float>();
			float fallDrag = playerControllerNode["fallDrag"].as<float>();

			// set data
			PlayerControllerComponent* comp = entity.ReplaceComponent<PlayerControllerComponent>();
			comp->radius = radius;
			comp->height = height;
			comp->speed = speed;
			comp->moveDrag = moveDrag;
			comp->fallDrag = fallDrag;
			comp->ptr = nullptr;
		}
	}

	inline void SceneSerialiser::DeserialiseSkybox(YAML::Node& root, Entity& entity)
	{
		YAML::Node skyboxNode = root["SkyboxComponent"];
		if (skyboxNode)
		{
			// get data
			bool active = skyboxNode["active"].as<bool>();
			std::string shader = skyboxNode["shader"].as<std::string>();
			std::vector<std::string> texturePaths;
			YAML::Node texturePathsNode = skyboxNode["texturePaths"];
			if (!texturePathsNode.IsSequence() || texturePathsNode.size() != 6)
			{
				AE_LOG_FATAL("Serialisation::DeserialiseSkybox::Failed -> Skybox textures must be a sequence of 6 textures");
			}

			for (Size_t i = 0; i < 6; i++)
			{
				texturePaths.push_back(texturePathsNode[i].as<std::string>());
			}

			// set data
			SkyboxComponent* comp = entity.ReplaceComponent<SkyboxComponent>();
			comp->active = active;
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
			comp->skybox = MakeShared<Skybox>(texturePaths);
		}
	}

	inline void SceneSerialiser::DeserialiseWater(YAML::Node& root, Entity& entity)
	{
		YAML::Node waterNode = root["WaterComponent"];
		if (waterNode)
		{
			// get data
			bool active = waterNode["active"].as<bool>();
			std::string shader = waterNode["shader"].as<std::string>();
			std::string dudv = waterNode["dudv"].as<std::string>();
			std::string normal = waterNode["normal"].as<std::string>();

			// set data
			WaterComponent* comp = entity.ReplaceComponent<WaterComponent>();
			comp->active = active;
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
			comp->dudv = AssetManager<Texture>::Instance().Get(dudv);
			comp->normal = AssetManager<Texture>::Instance().Get(normal);
			comp->water = MakeShared<Water>(comp->dudv, comp->normal);
		}
	}
}
