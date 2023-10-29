#include <fstream>
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/Identifier.h"
#include "AEngine/Core/Logger.h"
#include "Entity.h"
#include "SceneSerialiser.h"
#include "AEngine/Script/ScriptEngine.h"
#include "AEngine/Skybox/Skybox.h"
#include "AEngine/Water/Water.h"
#include "AEngine/Physics/PlayerController.h"

/// @todo Remove managers
#include "AEngine/Resource/AssetManager.h"

//--------------------------------------------------------------------------------
// Custom Nodes
//--------------------------------------------------------------------------------
namespace YAML
{
	template<>
	struct convert<AEngine::Math::ivec2> {
		static Node encode(const AEngine::Math::ivec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, AEngine::Math::ivec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			return true;
		}
	};

	template<>
	struct convert<AEngine::Math::vec4> {
		static Node encode(const AEngine::Math::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, AEngine::Math::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

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
	// initialise the scene pointer
	Scene* SceneSerialiser::s_scene = nullptr;

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
		scene->Init();

		s_scene = scene.get();
		DeserialiseNode(scene.get(), data);
		s_scene = nullptr;

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

	YAML::Node SceneSerialiser::SerialiseVec4(glm::vec4 vec)
	{
		YAML::Node node(YAML::NodeType::Sequence);
		node.SetStyle(YAML::EmitterStyle::Flow);
		node.push_back(vec.x);
		node.push_back(vec.y);
		node.push_back(vec.z);
		node.push_back(vec.w);
		return node;
	}

	YAML::Node SceneSerialiser::SerialiseVec3(glm::vec3 vec)
	{
		YAML::Node node(YAML::NodeType::Sequence);
		node.SetStyle(YAML::EmitterStyle::Flow);
		node.push_back(vec.x);
		node.push_back(vec.y);
		node.push_back(vec.z);
		return node;
	}

	YAML::Node SceneSerialiser::SerialiseVec2(glm::vec2 vec)
	{
		YAML::Node node(YAML::NodeType::Sequence);
		node.SetStyle(YAML::EmitterStyle::Flow);
		node.push_back(vec.x);
		node.push_back(vec.y);
		return node;
	}

	YAML::Node SceneSerialiser::SerialiseColliders(CollisionBody* body)
	{
		YAML::Node root;
		auto colliders = body->GetColliders();
		for (auto collider : colliders)
		{
			YAML::Node colliderNode;
			const char* type = collider->GetName();
			colliderNode["type"] = type;
			colliderNode["offset"] = SerialiseVec3(collider->GetOffset());
			colliderNode["orientation"] = SerialiseVec3(Math::degrees(Math::eulerAngles(collider->GetOrientation())));
			if (strcmp(type, "Box") == 0)
			{
				colliderNode["halfExtents"] = SerialiseVec3(dynamic_cast<BoxCollider*>(collider.get())->GetSize());
			}
			else if (strcmp(type, "Sphere") == 0)
			{
				colliderNode["radius"] = dynamic_cast<SphereCollider*>(collider.get())->GetRadius();
			}
			else if (strcmp(type, "Capsule") == 0)
			{
				colliderNode["radius"] = dynamic_cast<CapsuleCollider*>(collider.get())->GetRadius();
				colliderNode["height"] = dynamic_cast<CapsuleCollider*>(collider.get())->GetHeight();
			}
			root.push_back(colliderNode);
		}

		return root;
	}

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

		// fonts
		AssetManager<Font>& ff = AssetManager<Font>::Instance();
		std::map<std::string, SharedPtr<Font>>::const_iterator ffItr;
		for (ffItr = ff.begin(); ffItr != ff.end(); ++ffItr)
		{
			YAML::Node font;
			font["type"] = "font";
			font["path"] = ffItr->second->GetPath();
			assets.push_back(font);
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

		AssetManager<Grid>& gcrm = AssetManager<Grid>::Instance();
		std::map<std::string, SharedPtr<Grid>>::const_iterator gItr;
		for (gItr = gcrm.begin(); gItr != gcrm.end(); ++gItr)
		{
			YAML::Node grid;
			grid["type"] = "grid";
			grid["path"] = gItr->second->GetPath();
			assets.push_back(grid);
		}

		// textures
		root["assets"] = assets;

		//populate entities
		YAML::Node entities;

		// create map of entities to sort entities by tag
		std::map<std::string, entt::entity> entityMap;

		scene->m_Registry.view<TagComponent>().each([&](const auto entity, const auto& tag)
		{
			entityMap[tag.tag] = entity;
		});

		for (const auto &[mapkey, entity]: entityMap) {
			YAML::Node entityNode;

			// Tag Component
			if (scene->m_Registry.all_of<TagComponent>(entity))
			{
				TagComponent& tag = scene->m_Registry.get<TagComponent>(entity);
				YAML::Node tagNode;
				tagNode["tag"] = tag.tag;
				entityNode["TagComponent"] = tagNode;
			}

				// RectTransform Component
			if (scene->m_Registry.all_of<RectTransformComponent>(entity))
			{
				// get data
				RectTransformComponent& rectTransform = scene->m_Registry.get<RectTransformComponent>(entity);
				Math::vec3 translation = rectTransform.translation;
				Math::vec3 orientation = Math::eulerAngles(rectTransform.orientation);
				Math::vec3 scale = rectTransform.scale;
				Math::vec2 size = rectTransform.size;

				// convert orientation to degrees
				orientation.x = Math::degrees(orientation.x);
				orientation.y = Math::degrees(orientation.y);
				orientation.z = Math::degrees(orientation.z);

				// create node
				YAML::Node rectTransformNode;
				rectTransformNode["translation"] = SerialiseVec3(translation);
				rectTransformNode["orientation"] = SerialiseVec3(orientation);
				rectTransformNode["scale"] = SerialiseVec3(scale);
				rectTransformNode["size"] = SerialiseVec2(size);
				entityNode["RectTransformComponent"] = rectTransformNode;
			}
				// Transform Component
			else if (scene->m_Registry.all_of<TransformComponent>(entity))
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
				transformNode["translation"] = SerialiseVec3(translation);
				transformNode["orientation"] = SerialiseVec3(orientation);
				transformNode["scale"] = SerialiseVec3(scale);
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
			if (scene->m_Registry.all_of<SkinnedRenderableComponent>(entity))
			{
				// get data
				SkinnedRenderableComponent& animate = scene->m_Registry.get<SkinnedRenderableComponent>(entity);
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

				entityNode["SkinnedRenderableComponent"] = animateNode;
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

			// RigidBody Compnent
			if (scene->m_Registry.all_of<RigidBodyComponent>(entity))
			{
				// get data
				RigidBody* rb = (scene->m_Registry.get<RigidBodyComponent>(entity)).ptr.get();
				YAML::Node rigidNode;
				RigidBody::Type type = rb->GetType();
				std::string strType;

				if (type == RigidBody::Type::Dynamic)
				{
					strType = "dynamic";
				}
				else if (type == RigidBody::Type::Kinematic)
				{
					strType = "kinematic";
				}
				else if (type == RigidBody::Type::Static)
				{
					strType =  "static";
				}

				rigidNode["type"] = strType;
				rigidNode["hasGravity"] = rb->GetHasGravity();
				rigidNode["massKg"] = rb->GetMass();

				// colliders
				rigidNode["colliders"] = SerialiseColliders(rb);
				entityNode["RigidBodyComponent"] = rigidNode;
			}

			// CanvasRenderer Component
			if (scene->m_Registry.all_of<CanvasRendererComponent>(entity))
			{
				CanvasRendererComponent& canvas = scene->m_Registry.get<CanvasRendererComponent>(entity);
				bool active = canvas.active;
				bool screenspace = canvas.screenSpace;
				bool billboard = canvas.billboard;

				YAML::Node canvasNode;
				canvasNode["active"] = active;
				canvasNode["screen-space"] = screenspace;
				canvasNode["billboard"] = billboard;
				entityNode["CanvasRendererComponent"] = canvasNode;
			}

			// Text Component
			if (scene->m_Registry.all_of<TextComponent>(entity))
			{
				TextComponent& textComp = scene->m_Registry.get<TextComponent>(entity);
				std::string font = textComp.font->GetIdent();
				std::string text = textComp.text;
				Math::vec4 color = textComp.color;

				YAML::Node textNode;
				textNode["font"] = font;
				textNode["text"] = text;
				textNode["color"] = SerialiseVec4(color);
				entityNode["TextComponent"] = textNode;
			}

			// Image Component
			if (scene->m_Registry.all_of<PanelComponent>(entity))
			{
				PanelComponent& panel = scene->m_Registry.get<PanelComponent>(entity);
				std::string texture;
				if (panel.texture != nullptr)
					texture = panel.texture->GetIdent();
				else
					texture = "";
				Math::vec4 color = panel.color;

				YAML::Node panelNode;
				panelNode["texture"] = texture;
				panelNode["color"] = SerialiseVec4(color);
				entityNode["PanelComponent"] = panelNode;
			}

			if(scene->m_Registry.all_of<NavigationGridComponent>(entity))
			{
				NavigationGridComponent& nav = scene->m_Registry.get<NavigationGridComponent>(entity);
				bool debug = nav.debug;
				std::string ident = nav.grid->GetIdent();

				YAML::Node navNode;
				navNode["debug"] = debug;
				navNode["grid"] = ident;
				entityNode["NavigationGridComponent"] = navNode;
			}

			if(scene->m_Registry.all_of<PlayerControllerComponent>(entity))
			{
				PlayerControllerComponent& playerCon = scene->m_Registry.get<PlayerControllerComponent>(entity);

				Properties controlerProps = playerCon.ptr->GetControllerProperties();

				YAML::Node playerConNode;
				playerConNode["radius"] = controlerProps.radius;
				playerConNode["height"] = controlerProps.height;
				playerConNode["speed"] = controlerProps.moveFactor;
				playerConNode["moveDrag"] = controlerProps.moveDrag;
				playerConNode["fallDrag"] = controlerProps.fallDrag;
				playerConNode["offset"] = SerialiseVec3(controlerProps.capsuleOffset);
				entityNode["PlayerControllerComponent"] = playerConNode;
			}

			if(scene->m_Registry.all_of<SkyboxComponent>(entity))
			{
				SkyboxComponent& skybox = scene->m_Registry.get<SkyboxComponent>(entity);

				YAML::Node skyboxNode;
				skyboxNode["active"] = skybox.active;
				skyboxNode["shader"] = skybox.shader->GetIdent();
				skyboxNode["texturePaths"] = skybox.skybox->GetTexturePaths();
				entityNode["SkyboxComponent"] = skyboxNode;
			}

			entities.push_back(entityNode);
		};

		root["entities"] = entities;
		return root;
	}

	//--------------------------------------------------------------------------------
	// File Deserialisation
	//--------------------------------------------------------------------------------

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
				SceneSerialiser::DeserialiseCollisionBody(entityNode, entity);
				SceneSerialiser::DeserialiseRigidBody(entityNode, entity);

				SceneSerialiser::DeserialiseRenderable(entityNode, entity);
				SceneSerialiser::DeserialiseSkinnedRenderable(entityNode, entity);
				SceneSerialiser::DeserialiseCamera(entityNode, entity);
				SceneSerialiser::DeserialiseBDIAgent(entityNode, entity);  //<  must be before script

				SceneSerialiser::DeserialiseScript(entityNode, entity);
				SceneSerialiser::DeserialisePlayerController(entityNode, entity);
				SceneSerialiser::DeserialiseSkybox(entityNode, entity);

				SceneSerialiser::DeserialiseNavigationGridComponent(entityNode, entity);

				SceneSerialiser::DeserialiseRectTransform(entityNode, entity);
				SceneSerialiser::DeserialiseCanvasRenderer(entityNode, entity);
				SceneSerialiser::DeserialiseText(entityNode, entity);
				SceneSerialiser::DeserialiseImage(entityNode, entity);
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
		else if (type == "font")
		{
			AssetManager<Font>::Instance().Load(path);
		}
		else if (type == "grid")
		{
			AssetManager<Grid>::Instance().Load(path);
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

	inline void SceneSerialiser::DeserialiseNavigationGridComponent(YAML::Node& root, Entity& entity)
	{
		YAML::Node navNode = root["NavigationGridComponent"];
		if(navNode)
		{
			bool debug = navNode["debug"].as<bool>();
			std::string ident = navNode["grid"].as<std::string>();

			NavigationGridComponent* comp = entity.ReplaceComponent<NavigationGridComponent>();
			comp->debug = debug;

			if(ident == "null")
				comp->grid = Grid::Create(Math::ivec2(0), 0.0f, Math::vec3(0.0f));
			else
				comp->grid = AssetManager<Grid>::Instance().Get(ident);
		}
	}

	inline void SceneSerialiser::DeserialiseRectTransform(YAML::Node& root, Entity& entity)
	{
		YAML::Node rectTransformNode = root["RectTransformComponent"];
		if(rectTransformNode)
		{
			// get data
			Math::vec3 translation = rectTransformNode["translation"].as<Math::vec3>();
			Math::vec3 orientation = rectTransformNode["orientation"].as<Math::vec3>();
			Math::vec3 scale = rectTransformNode["scale"].as<Math::vec3>();
			Math::vec2 size = rectTransformNode["size"].as<Math::vec2>();

			// convert orientation to radians
			orientation.x = Math::radians(orientation.x);
			orientation.y = Math::radians(orientation.y);
			orientation.z = Math::radians(orientation.z);

			// set data
			RectTransformComponent* comp = entity.ReplaceComponent<RectTransformComponent>();
			comp->translation = translation;
			comp->orientation = orientation;
			comp->scale = scale;
			comp->size = size;
		}
	}

	inline void SceneSerialiser::DeserialiseCanvasRenderer(YAML::Node& root, Entity& entity)
	{
		YAML::Node canvasNode = root["CanvasRendererComponent"];
		if(canvasNode)
		{
			bool active = canvasNode["active"].as<bool>();
			bool screenspace = canvasNode["screen-space"].as<bool>();
			bool billboard = canvasNode["billboard"].as<bool>();

			CanvasRendererComponent* comp = entity.ReplaceComponent<CanvasRendererComponent>();

			comp->active = active;
			comp->screenSpace = screenspace;
			comp->billboard = billboard;
		}
	}

	inline void SceneSerialiser::DeserialiseText(YAML::Node& root, Entity& entity)
	{
		YAML::Node textNode = root["TextComponent"];
		if(textNode)
		{
			std::string font = textNode["font"].as<std::string>();
			std::string text = textNode["text"].as<std::string>();
			Math::vec4 color = textNode["color"].as<Math::vec4>();

			TextComponent* comp = entity.ReplaceComponent<TextComponent>();

			comp->font = AssetManager<Font>::Instance().Get(font);
			comp->text = text;
			comp->color = color;
		}
	}

	inline void SceneSerialiser::DeserialiseImage(YAML::Node& root, Entity& entity)
	{
		YAML::Node imageNode = root["PanelComponent"];
		if(imageNode)
		{
			std::string texture = imageNode["texture"].as<std::string>();
			Math::vec4 color = imageNode["color"].as<Math::vec4>();

			PanelComponent* comp = entity.ReplaceComponent<PanelComponent>();

			comp->texture = AssetManager<Texture>::Instance().Get(texture);
			comp->color = color;
		}
	}

	void SceneSerialiser::DeserialiseBDIAgent(YAML::Node &root, Entity &entity)
	{
		YAML::Node bdiNode = root["BDIComponent"];
		if (bdiNode)
		{
			std::string agentName = bdiNode["agentName"].as<std::string>();
			BDIComponent* comp = entity.ReplaceComponent<BDIComponent>();
			comp->ptr = MakeShared<BDIAgent>(agentName);
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

	inline void SceneSerialiser::DeserialiseSkinnedRenderable(YAML::Node& root, Entity& entity)
	{
		YAML::Node animateNode = root["SkinnedRenderableComponent"];
		if (animateNode)
		{
			// get data
			bool active = animateNode["active"].as<bool>();
			std::string model = animateNode["model"].as<std::string>();
			std::string shader = animateNode["shader"].as<std::string>();
			std::string animation = animateNode["startAnimation"].as<std::string>();

			// set data
			SkinnedRenderableComponent* comp = entity.ReplaceComponent<SkinnedRenderableComponent>();


			comp->active = active;
			comp->model = AssetManager<Model>::Instance().Get(model);
			comp->shader = AssetManager<Shader>::Instance().Get(shader);
			comp->animator.Load(*AssetManager<Animation>::Instance().Get(animation));
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
			if (entity.HasComponent<CollisionBodyComponent>())
			{
				AE_LOG_FATAL("Serialisation::DeserialiseRigidBody::Failed -> Entity cannot have both a rigid body and a collision body");
			}

			TransformComponent* transform = entity.GetComponent<TransformComponent>();
			RigidBodyComponent* comp = entity.AddComponent<RigidBodyComponent>();
			comp->ptr = s_scene->m_physicsWorld->AddRigidBody(transform->translation, transform->orientation);

			// get data
			if (rigidBodyNode["type"])
			{
				std::string strType = rigidBodyNode["type"].as<std::string>();
				if (strType == "dynamic")
				{
					comp->ptr->SetType(RigidBody::Type::Dynamic);
				}
				else if (strType == "kinematic")
				{
					comp->ptr->SetType(RigidBody::Type::Kinematic);
				}
				else if (strType == "static")
				{
					comp->ptr->SetType(RigidBody::Type::Static);
				}
				else
				{
					AE_LOG_FATAL("Serialisation::DeserialiseRigidBody::Failed -> Type '{}' doesn't exist", strType);
				}
			}

			if (rigidBodyNode["massKg"])
			{
				comp->ptr->SetMass(rigidBodyNode["massKg"].as<float>());
			}

			if (rigidBodyNode["hasGravity"])
			{
				comp->ptr->SetHasGravity(rigidBodyNode["hasGravity"].as<bool>());
			}

			if (rigidBodyNode["restitution"])
			{
				comp->ptr->SetRestitution(rigidBodyNode["restitution"].as<float>());
			}

			if (rigidBodyNode["linearDamping"])
			{
				comp->ptr->SetLinearDamping(rigidBodyNode["linearDamping"].as<float>());
			}

			if (rigidBodyNode["angularDamping"])
			{
				comp->ptr->SetAngularDamping(rigidBodyNode["angularDamping"].as<float>());
			}

			if (rigidBodyNode["linearVelocity"])
			{
				comp->ptr->SetLinearVelocity(rigidBodyNode["linearVelocity"].as<Math::vec3>());
			}

			if (rigidBodyNode["angularVelocity"])
			{
				comp->ptr->SetAngularVelocity(rigidBodyNode["angularVelocity"].as<Math::vec3>());
			}

			YAML::Node colliders = rigidBodyNode["colliders"];
			if(colliders)
			{
				DeserialiseCollider(colliders, comp->ptr.get());
			}
		}
	}

	inline void SceneSerialiser::DeserialiseCollisionBody(YAML::Node& root, Entity& entity)
	{
		YAML::Node collisionBodyNode = root["CollisionBodyComponent"];
		if (collisionBodyNode)
		{
			if (entity.HasComponent<RigidBodyComponent>())
			{
				AE_LOG_FATAL("Serialisation::DeserialiseCollisionBody::Failed -> Entity cannot have both a rigid body and a collision body");
			}

			// get transform component of entity
			TransformComponent* transform = entity.GetComponent<TransformComponent>();

			// create component and register with physics world
			CollisionBodyComponent* comp = entity.AddComponent<CollisionBodyComponent>();
			comp->ptr = s_scene->m_physicsWorld->AddCollisionBody(transform->translation, transform->orientation);

			// parse colliders
			YAML::Node colliders = collisionBodyNode["colliders"];
			if (colliders)
			{
				DeserialiseCollider(colliders, comp->ptr.get());
			}
		}
	}

	inline void SceneSerialiser::DeserialiseCollider(YAML::Node& colliderNode, CollisionBody* body)
	{
		// check validity of colliders
		if (!colliderNode.IsSequence())
		{
			AE_LOG_FATAL("Serialisation::DeserialiseCollisionBody::Failed -> Colliders must be a sequence");
		}

		// for each collider, add it to the collision body
		for (int i = 0; i < colliderNode.size(); i++)
		{
			YAML::Node collider = colliderNode[i];
			std::string type = collider["type"].as<std::string>();
			Math::quat orientation = Math::quat(Math::radians(collider["orientation"].as<Math::vec3>()));
			Math::vec3 offset = collider["offset"].as<Math::vec3>();
			if (type == "Box")
			{
				Math::vec3 halfExtents = collider["halfExtents"].as<Math::vec3>();
				body->AddBoxCollider(halfExtents, offset, orientation);
			}
			else if (type == "Sphere")
			{
				float radius = collider["radius"].as<float>();
				body->AddSphereCollider(radius, offset, orientation);
			}
			else if (type == "Capsule")
			{
				float radius = collider["radius"].as<float>();
				float height = collider["height"].as<float>();
				body->AddCapsuleCollider(radius, height, offset, orientation);
			}
			else
			{
				AE_LOG_FATAL("Serialisation::DeserialiseCollisionBody::Failed -> Collider type '{}' doesn't exist", type);
			}
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
			Math::vec3 offset = playerControllerNode["offset"].as<Math::vec3>();

			// set data
			TransformComponent* tc = entity.GetComponent<TransformComponent>();
			PlayerControllerComponent* comp = entity.ReplaceComponent<PlayerControllerComponent>();
			comp->ptr = new PlayerController(
				s_scene->GetPhysicsWorld(),
				tc->translation,
				{ radius, height, speed, moveDrag, fallDrag, offset }
			);
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
}
