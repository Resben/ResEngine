#include <fstream>
#include <yaml-cpp/yaml.h>
#include "AEngine/Core/Logger.h"
#include "Entity.h"
#include "SceneSerialiser.h"

/// @todo Remove managers
#include "AEngine/Resource/ModelManager.h"
#include "AEngine/Resource/ShaderManager.h"
#include "AEngine/Resource/TextureManager.h"

namespace AEngine
{
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
		AE_LOG_ERROR("SceneSerialiser::SerialiseFile::Error -> Not Implemented");

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

	YAML::Node SceneSerialiser::SerialiseNode(Scene* scene)
	{
		YAML::Node root;

		// scene ident
		root["scene"] = scene->m_ident;

		// populate assets
		YAML::Node assets;
		// models
		ModelManager* mm = ModelManager::Instance();
		std::map<std::string, std::shared_ptr<Model>>::const_iterator modItr;
		for (modItr = mm->begin(); modItr != mm->end(); ++modItr)
		{
			YAML::Node model;
			model["type"] = "model";
			model["path"] = modItr->second->GetPath();
			assets.push_back(model);
		}

		// shaders
		ShaderManager* sm = ShaderManager::Instance();
		std::map<std::string, std::shared_ptr<Shader>>::const_iterator sdrItr;
		for (sdrItr = sm->begin(); sdrItr != sm->end(); ++sdrItr)
		{
			YAML::Node shader;
			shader["type"] = "shader";
			shader["path"] = sdrItr->second->GetPath();
			assets.push_back(shader);
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
				TransformComponent& transform = scene->m_Registry.get<TransformComponent>(entity);
				Math::vec3 translation = transform.translation;
				Math::vec3 rotation = Math::eulerAngles(transform.rotation);
				Math::vec3 scale = transform.scale;

				// create translation node
				YAML::Node translationNode;
				translationNode.push_back(translation.x);
				translationNode.push_back(translation.y);
				translationNode.push_back(translation.z);

				// create rotation node
				YAML::Node rotationNode;
				rotationNode.push_back(Math::degrees(rotation.x));
				rotationNode.push_back(Math::degrees(rotation.y));
				rotationNode.push_back(Math::degrees(rotation.z));

				// create scale node
				YAML::Node scaleNode;
				scaleNode.push_back(scale.x);
				scaleNode.push_back(scale.y);
				scaleNode.push_back(scale.z);

				// create transform node
				YAML::Node transformNode;
				transformNode["translation"] = translationNode;
				transformNode["rotation"] = rotationNode;
				transformNode["scale"] = scaleNode;
				entityNode["TransformComponent"] = transformNode;
			}

			// Renderable Component
			if (scene->m_Registry.all_of<RenderableComponent>(entity))
			{
				RenderableComponent& renderable = scene->m_Registry.get<RenderableComponent>(entity);
				bool isActive = renderable.active;
				std::string model = renderable.model->GetIdent();
				std::string shader = renderable.shader->GetIdent();

				YAML::Node renderNode;
				renderNode["active"] = isActive;
				renderNode["model"] = model;
				renderNode["shader"] = shader;
				entityNode["RenderableComponent"] = renderNode;
			}

			// Camera Component
			if (scene->m_Registry.all_of<CameraComponent>(entity))
			{
				CameraComponent& camera = scene->m_Registry.get<CameraComponent>(entity);
				bool isActive = camera.active;
				float fov = camera.camera.GetFov();
				float aspect = camera.camera.GetAspect();
				float nearPlane = camera.camera.GetNearPlane();
				float farPlane = camera.camera.GetFarPlane();

				YAML::Node camConfig;
				camConfig["fov"] = fov;
				camConfig["aspect"] = aspect;
				camConfig["nearPlane"] = nearPlane;
				camConfig["farPlane"] = farPlane;

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
		// load assets
		YAML::Node assets = data["assets"];
		if (assets)
		{
			for (YAML::Node assetNode : assets)
			{
				std::string type = assetNode["type"].as<std::string>();
				std::string path = assetNode["path"].as<std::string>();

				if (type == "model")
				{
					ModelManager::Instance()->LoadModel(path);
				}
				else if (type == "shader")
				{
					ShaderManager::Instance()->LoadShader(path);
				}
				else if (type == "texture")
				{
					TextureManager::Instance()->LoadTexture(path);
				}
				else
				{
					AE_LOG_FATAL("Serialisation::Load::Asset::Failed -> Type '{}' doesn't exist", type);
				}
			}
		}

		// generate entities
		YAML::Node entities = data["entities"];
		if (entities)
		{
			for (YAML::Node entityNode : entities)
			{
				// create entity with tag
				std::string name;
				YAML::Node tagComp = entityNode["TagComponent"];
				if (tagComp)
				{
					name = tagComp["tag"].as<std::string>();
				}

				Entity entity = scene->CreateEntity(name);

				// transform component
				YAML::Node transComp = entityNode["TransformComponent"];
				if (transComp)
				{
					YAML::Node translation = transComp["translation"];
					Math::vec3 translationVec = {
						translation[0].as<float>(),
						translation[1].as<float>(),
						translation[2].as<float>()
					};

					YAML::Node rotation = transComp["rotation"];
					Math::vec3 rotationVec = {
						Math::radians(rotation[0].as<float>()),
						Math::radians(rotation[1].as<float>()),
						Math::radians(rotation[2].as<float>())
					};

					YAML::Node scale = transComp["scale"];
					Math::vec3 scaleVec = {
						scale[0].as<float>(),
						scale[1].as<float>(),
						scale[2].as<float>()
					};

					TransformComponent* comp = entity.AddComponent<TransformComponent>();
					comp->translation = translationVec;
					comp->rotation = rotationVec;
					comp->scale = scaleVec;
				}

				// Renderable component
				YAML::Node renderableComp = entityNode["RenderableComponent"];
				if (renderableComp)
				{
					// get config
					bool active = renderableComp["active"].as<bool>();
					std::string model = renderableComp["model"].as<std::string>();
					std::string shader = renderableComp["shader"].as<std::string>();

					// apply to entity
					RenderableComponent* comp = entity.AddComponent<RenderableComponent>();
					comp->active = active;
					comp->model = ModelManager::Instance()->GetModel(model);
					comp->shader = ShaderManager::Instance()->GetShader(shader);

				}

				YAML::Node cameraComp = entityNode["CameraComponent"];
				if (cameraComp)
				{
					// get config
					bool active = cameraComp["active"].as<bool>();
					YAML::Node cameraSettings = cameraComp["camera"];
					float fov = cameraSettings["fov"].as<float>();
					float aspect = cameraSettings["aspect"].as<float>();
					float nearPlane = cameraSettings["nearPlane"].as<float>();
					float farPlane = cameraSettings["farPlane"].as<float>();

					// apply to entity
					CameraComponent* comp = entity.AddComponent<CameraComponent>();
					comp->active = active;
					comp->camera = PerspectiveCamera(fov, aspect, nearPlane, farPlane);
				}
			}
		}
	}
}