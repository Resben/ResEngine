#include "Editor.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Events/EventHandler.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"

#include "AEngine/Scene/Entity.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/SceneManager.h"
#include "AEngine/Core/Logger.h"

#include <iostream>

namespace AEngine
{
	void Editor::Init(Window *window, const EditorProperties& props)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();

		// How to deal with mouse input; arguably don't need to unset
		// the keyboard (no option) as it won't be polled without the mouse hover
		// -> io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
		// Maybe add a boolean for the `viewport capturing` input, which can
		// be used to determine how this setting is set/unset

		io.ConfigWindowsMoveFromTitleBarOnly = props.TitleBarMove;
		if(props.IsDockingEnabled)
		{
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		}

		if(props.IsViewportEnabled)
		{
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNative()), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();

		ImGuiStyle& guiStyle = ImGui::GetStyle();
		guiStyle.WindowMenuButtonPosition = ImGuiDir_None;
		guiStyle.Alpha = 0.8f;

		// Handle events from the window and pass to game layer if needed
		window->RegisterEventHandler<KeyPressed>(0, [&io, this](KeyPressed& e) -> bool {
			return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<KeyReleased>(0, [this](KeyReleased& e) -> bool {
			return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse;
		});

		window->RegisterEventHandler<MouseButtonPressed>(0, [this](MouseButtonPressed& e) -> bool {
			return ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseButtonReleased>(0, [this](MouseButtonReleased& e) -> bool {
			return ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseMoved>(0, [this](MouseMoved& e) -> bool {
			return ImGui::GetIO().WantCaptureMouse;
		});
	}

	void Editor::CreateNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::Update()
	{
		/**
		 * \todo Think about implementing observer pattern to send out updates
		 * when the scene is changed. This would need to be added to the
		 * SceneManager class.
		*/
		m_scene = SceneManager::GetActiveScene();
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// ShowGameViewPort();
		ShowHierarchy();
		ShowInspector();
		ShowDebugWindow();
		ShowDebugCameraConfig();

		// ImGui::End();
	}

	void Editor::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO &io = ImGui::GetIO();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//get our current window something like
			Window* window = Application::Instance().GetWindow();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			//change back to our window rendering context?
			window->MakeCurrent();
		}
	}

	void Editor::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::ShowDebugCameraConfig()
	{
		// Get attributes
		DebugCamera& debugCam = Scene::GetDebugCamera();
		// Perspective camera
		float fov = debugCam.GetFov();
		float aspect = debugCam.GetAspect();
		float nearPlane = debugCam.GetNearPlane();
		float farPlane = debugCam.GetFarPlane();
		// Debug camera
		Math::vec3 pos = debugCam.GetPosition();
		float movementSpeed = debugCam.GetMovementSpeed();
		float lookSensitivity = debugCam.GetLookSensitivity();

		ImGui::Begin("Editor Camera Config");
		ImGui::Text("Position: (%.3f, %.3f, %.3f)", pos.x, pos.y, pos.z);
		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::SliderFloat("FOV", &fov, 10.0f, 180.0f, "%.3f");
		ImGui::SliderFloat("Aspect", &aspect, 0.1f, 10.0f, "%.3f");
		ImGui::SliderFloat("Near Plane", &nearPlane, 0.1f, 10.0f, "%.3f");
		ImGui::SliderFloat("Far Plane", &farPlane, 10.0f, 10000.0f, "%.3f");
		ImGui::Separator();
		ImGui::Text("Movement");
		ImGui::SliderFloat("Movement Speed", &movementSpeed, 1.0f, 100.0f, "%.3f");
		ImGui::SliderFloat("Look Sensitivity", &lookSensitivity, 0.1f, 10.0f, "%.3f");
		ImGui::End();

		// Set attributes
		debugCam.SetFov(fov);
		debugCam.SetAspect(aspect);
		debugCam.SetNearPlane(nearPlane);
		debugCam.SetFarPlane(farPlane);
		debugCam.SetPosition(pos);
		debugCam.SetMovementSpeed(movementSpeed);
		debugCam.SetLookSensitivity(lookSensitivity);
	}

	void Editor::ShowDebugWindow()
	{
		ImGui::Begin("Editor Debug");
		// group of radio buttons for scene state
		ImGui::Text("Select Scene State");
		ImGui::Spacing();
		int sceneState = static_cast<int>(m_scene->GetState());
		ImGui::RadioButton("Edit", &sceneState, static_cast<int>(Scene::State::Edit));
		ImGui::SameLine();
		ImGui::RadioButton("Simulate", &sceneState, static_cast<int>(Scene::State::Simulate));
		ImGui::SameLine();
		ImGui::RadioButton("Pause", &sceneState, static_cast<int>(Scene::State::Pause));
		m_scene->SetState(static_cast<Scene::State>(sceneState));
		ImGui::Separator();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Load Scene"))
				{
					// load scene
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					SceneManager::SaveActiveToFile("serialized.scene");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void Editor::ShowHierarchy()
	{
		constexpr float removeButtonWidth = 20.0f;
		constexpr float removeButtonPadding = 10.0f;

		std::vector<Uint16> entityids;
		m_scene->GetEntityIds(entityids);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		ImGui::Begin(std::string("Hierarchy " + m_scene->GetIdent()).c_str());
		if (ImGui::BeginPopup("Add Entity Popup"))
		{
			bool accept = false;
			static char name[32] = "Entity";
			ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
			if (ImGui::Button("Add", ImVec2(120, 0)))
			{
				accept = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				accept = false;
				ImGui::CloseCurrentPopup();
			}

			if (accept)
			{
				// Add entity to scene, with a default transform component
				Entity newEntity = m_scene->CreateEntity(name);
				newEntity.AddComponent<TransformComponent>();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Add Entity"))
		{
			ImGui::OpenPopup("Add Entity Popup");
		}

		for(int i = 0; i < entityids.size(); i++)
		{
			Entity entity = m_scene->GetEntity(entityids[i]);
			std::string entityName = entity.GetComponent<TagComponent>()->tag;
			if (entity != m_selectedEntity)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
				const float buttonWidth = ImGui::GetContentRegionAvail().x - (removeButtonWidth + removeButtonPadding);
				if(ImGui::Button(entityName.c_str(), ImVec2(buttonWidth, 0)))
				{
					m_selectedEntity = m_scene->GetEntity(entityids[i]);
				}
				ImGui::PopStyleVar();
			}
			else
			{
				// Is selected entity, so just replace with coloured text
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
				ImGui::Text(entityName.c_str());
				ImGui::PopStyleColor();
			}

			// Add a remove button
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 0.8f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 0.6f));
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (removeButtonWidth + removeButtonPadding));
			if (ImGui::Button(std::string("X##" + entityName).c_str(), ImVec2(20, 0)))
			{
				// remove entity and reset selected entity
				m_scene->RemoveEntity(entityName);
				m_selectedEntity = Entity();
			}
			ImGui::PopStyleColor(3);
		}
		ImGui::End();

		ImGui::PopStyleColor(3);
	}

	void Editor::ShowInspector()
	{
		ImGui::Begin("Inspector");

		// No entity selected message
		if (!m_selectedEntity.IsValid())
		{
			ImGui::Text("No entity selected");
			ImGui::End();
			return;
		}

		ShowTagComponent();
		ShowTransformComponent();
		ShowRenderableComponent();
		ShowSkinnedRenderableComponent();
		ShowSkyboxComponent();
		ShowCameraComponent();
		ShowScriptableComponent();
		ShowRigidBodyComponent();
		ShowCollisionBodyComponent();
		ShowPlayerControllerComponent();
		ImGui::Spacing();
		ImGui::Spacing();
		ShowAddComponentButton();
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}


		//might need to have similar function for each entity and check against entity to view everything
		//build out for all components

		ImGui::End();
	}

	void Editor::ShowAddComponentButton()
	{
		assert (m_selectedEntity.IsValid());

		if (ImGui::BeginPopup("Add Component"))
		{
			ShowAddComponentPrompt<CollisionBodyComponent>("Collision Body");
			ShowAddComponentPrompt<RenderableComponent>("Renderable");
			ImGui::EndPopup();
		}
	}

	template <typename T>
	void Editor::ShowAddComponentPrompt(const char* label)
	{
		if (!m_selectedEntity.HasComponent<T>())
		{
			if (ImGui::MenuItem(label))
			{
				m_selectedEntity.AddComponent<T>();
			}
		}
	}

	void Editor::ShowTagComponent()
	{
		TagComponent* tc = m_selectedEntity.GetComponent<TagComponent>();
		if(tc != nullptr)
		{
			ImGui::Text("Name: %s", tc->tag.c_str());
			ImGui::Text("ID: %d", tc->ident);
		}
	}

	void Editor::ShowTransformComponent()
	{
		TransformComponent* tc = m_selectedEntity.GetComponent<TransformComponent>();
		if(tc != nullptr)
		{
			if(ImGui::CollapsingHeader("Transform Component"))
			{
				Math::vec3* translation =  &tc->translation;
				ImGui::DragFloat3("Translation", &(translation->x), 1.0f, 0.0f, 0.0f, "%.3f");

				Math::quat* orientation = &tc->orientation;
        		Math::vec3 eulerAnglesDegrees = Math::degrees(Math::eulerAngles(*orientation));
        		ImGui::DragFloat3("Rotation", &eulerAnglesDegrees.x, 1.0f, 0.0f, 0.0f, "%.3f");
        		*orientation = Math::quat(Math::radians(eulerAnglesDegrees));

				Math::vec3* scale = &tc->scale;
				ImGui::DragFloat3("Scale", &(scale->x), 1.0f, 0.0f, FLT_MAX, "%.3f");
			}
		}
	}

	void Editor::ShowRenderableComponent()
	{
		RenderableComponent* rc = m_selectedEntity.GetComponent<RenderableComponent>();
		if(rc != nullptr)
		{
			if (ImGui::BeginPopup("Model Selection"))
			{
				// show all models in asset manager
				std::map<std::string, SharedPtr<Model>>::const_iterator it;
				for (it = AssetManager<Model>::Instance().begin(); it != AssetManager<Model>::Instance().end(); ++it)
				{
					if (ImGui::MenuItem(it->first.c_str()))
					{
						rc->model = it->second;
					}
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Shader Selection"))
			{
				// show all models in asset manager
				std::map<std::string, SharedPtr<Shader>>::const_iterator it;
				for (it = AssetManager<Shader>::Instance().begin(); it != AssetManager<Shader>::Instance().end(); ++it)
				{
					if (ImGui::MenuItem(it->first.c_str()))
					{
						rc->shader = it->second;
					}
				}
				ImGui::EndPopup();
			}

			Model* model = rc->model.get();
			Shader* shader = rc->shader.get();
			if(ImGui::CollapsingHeader("Renderable Component"))
			{
				const std::string& modelIdent = model ? model->GetIdent() : "None";
				const std::string& shaderIdent = shader ? shader->GetIdent() : "None";

				ImGui::Checkbox("IsActive", &(rc->active));
				ImGui::Text("Model");
				ImGui::SameLine();
				if (ImGui::Button(modelIdent.c_str()))
				{
					ImGui::OpenPopup("Model Selection");
				}

				ImGui::Text("Shader");
				ImGui::SameLine();
				if (ImGui::Button(shaderIdent.c_str()))
				{
					ImGui::OpenPopup("Shader Selection");
				}
			}
		}
	}

	void Editor::ShowSkinnedRenderableComponent()
	{
		SkinnedRenderableComponent* src = m_selectedEntity.GetComponent<SkinnedRenderableComponent>();
		if(src != nullptr)
		{
			if(ImGui::CollapsingHeader("Skinned Renderable Component"))
			{
				ImGui::Checkbox("IsActive", &(src->active));
				//model and shader and animator
			}
		}
	}

	void Editor::ShowSkyboxComponent()
	{
		SkyboxComponent* sb = m_selectedEntity.GetComponent<SkyboxComponent>();
		if(sb != nullptr)
		{
			if(ImGui::CollapsingHeader("SkyBox Component"))
			{
				ImGui::Checkbox("Is Active", &(sb->active));
				//skybox
				//shader
			}
		}
	}

	void Editor::ShowCameraComponent()
	{
		CameraComponent* cc = m_selectedEntity.GetComponent<CameraComponent>();
		if(cc != nullptr)
		{
			if(ImGui::CollapsingHeader("Camera Component"))
			{
				//Perspective Camera
			}
		}
	}

	void Editor::ShowScriptableComponent()
	{
		ScriptableComponent* sc = m_selectedEntity.GetComponent<ScriptableComponent>();
		if(sc != nullptr)
		{
			if(ImGui::CollapsingHeader("Scriptable Component"))
			{
				//script
			}
		}
	}

	void Editor::ShowCollisionBodyComponent()
	{
		CollisionBodyComponent* cc = m_selectedEntity.GetComponent<CollisionBodyComponent>();
		if(cc != nullptr)
		{
			TransformComponent* tc = m_selectedEntity.GetComponent<TransformComponent>();

			/// \todo Rework the way collision bodies are created in the editor
			/// For now, create a new collision body if one doesn't exist
			if (!cc->ptr)
			{
				PhysicsWorld* world = m_scene->GetPhysicsWorld();
				cc->ptr= world->AddCollisionBody(tc->translation, tc->orientation);
			}

			if(ImGui::CollapsingHeader("CollisionBody Component"))
			{
				// Add collider popup
				if (ImGui::BeginPopup("Add Collider Popup##CollisionBody"))
				{
					if (ImGui::MenuItem("Box Collider"))
					{
						cc->ptr->AddBoxCollider(Math::vec3(1.0f, 1.0f, 1.0f), Math::vec3(0.0f, 0.0f, 0.0f), tc->orientation);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				// Add collider button
				if (ImGui::Button("Add Collider##CollisionBody"))
				{
					ImGui::OpenPopup("Add Collider Popup##CollisionBody");
				}

				// list the colliders
				CollisionBody* cb = cc->ptr.get();
				UniquePtr<Collider> collider = cb->GetCollider();
				if (collider)
				{
					bool isTrigger = collider->GetIsTrigger();
					ImGui::Text("Colliders");
					ImGui::Separator();
					ImGui::Text("Type: %s", collider ? collider->GetName() : "None");
					ImGui::Checkbox("Is Trigger", &isTrigger);
					collider->SetIsTrigger(isTrigger);

					// General configurations
					Math::vec3 offset = collider->GetOffset();
					Math::vec3 orientation = Math::degrees(Math::eulerAngles(collider->GetOrientation()));
					ImGui::DragFloat3("Offset", &offset.x, 0.1f, 0.0f, 0.0f, "%.3f");
					ImGui::DragFloat3("Orientation", &orientation.x, 0.1f, 0.0f, 0.0f, "%.3f");
					collider->SetOffset(offset);
					collider->SetOrientation(Math::quat(Math::radians(orientation)));
					ImGui::Separator();
					// Box collider configurations
					if (collider->GetType() == Collider::Type::Box)
					{
						Math::vec3 size = dynamic_cast<BoxCollider*>(collider.get())->GetSize();
						ImGui::DragFloat3("Size", &size.x, 0.1f, 0.0f, FLT_MAX, "%.3f");
						dynamic_cast<BoxCollider*>(collider.get())->Resize(size);
					}
				}
			}
		}
	}


	void Editor::ShowRigidBodyComponent()
	{
		RigidBodyComponent* rc = m_selectedEntity.GetComponent<RigidBodyComponent>();
		if(rc != nullptr)
		{
			if(ImGui::CollapsingHeader("Rigid Body Component"))
			{
				ImGui::InputFloat("Mass:", &(rc->massKg), 0.01f, 0.1f, "%.3f");
				ImGui::Checkbox("Gravity:", &(rc->hasGravity));
				ImGui::Text("Rigid Body Type %s", &(rc->type));
			}
		}
	}

	void Editor::ShowPlayerControllerComponent()
	{
		PlayerControllerComponent* pcc = m_selectedEntity.GetComponent<PlayerControllerComponent>();
		if(pcc != nullptr)
		{
			if(ImGui::CollapsingHeader("Player Controller Component"))
			{
				ImGui::InputFloat("Radius: ", &(pcc->radius), 0.01f, 0.1f, "%.3f");
				ImGui::InputFloat("Height: ", &(pcc->height), 0.01f, 0.1f, "%.3f");
				ImGui::InputFloat("Speed: ", &(pcc->speed), 0.01f, 0.1f, "%.3f");
				ImGui::InputFloat("Move Drag: ", &(pcc->moveDrag), 0.01f, 0.1f, "%.3f");
				ImGui::InputFloat("Fall Drag: ", &(pcc->fallDrag), 0.01f, 0.1f, "%.3f");
			}
		}
	}
}
