#include "Editor.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

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
	bool captureKeyDown = false;
	bool captureKeyUp = false;
	bool captureMouseClick = false;
	bool captureMouseRelease = false;
	bool captureMouseMovement = false;

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

		/// \todo Remove this once the viewport is implemented
		guiStyle.Alpha = 0.7f;

		// Handle events from the window and pass to game layer if needed
		window->RegisterEventHandler<KeyPressed>(0, [&io, this](KeyPressed& e) -> bool {
			return captureKeyDown = !m_viewportFocused && ImGui::GetIO().WantCaptureKeyboard;
		});
		window->RegisterEventHandler<KeyReleased>(0, [this](KeyReleased& e) -> bool {
			return captureKeyUp = !m_viewportFocused && ImGui::GetIO().WantCaptureKeyboard;
		});

		window->RegisterEventHandler<MouseButtonPressed>(0, [this](MouseButtonPressed& e) -> bool {
			// if the mouse is clicked in the viewport, focus the viewport and start the scene
			if (m_viewportHovered && (e.GetButton() == AEMouse::BUTTON_RIGHT))
			{
				m_viewportFocused = true;
				Application::Instance().GetWindow()->ShowCursor(false);
			}

			return captureMouseClick = !m_viewportFocused && ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseButtonReleased>(0, [this](MouseButtonReleased& e) -> bool {
			if (m_viewportFocused && (e.GetButton() == AEMouse::BUTTON_RIGHT))
			{
				m_viewportFocused = false;
				Application::Instance().GetWindow()->ShowCursor(true);
			}

			return captureMouseRelease = !m_viewportFocused && ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseMoved>(0, [this](MouseMoved& e) -> bool {
			return captureMouseMovement = !m_viewportFocused && ImGui::GetIO().WantCaptureMouse;
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
		m_viewportHovered = false;

		/// \todo think about implementing observer pattern
		m_scene = SceneManager::GetActiveScene();

		// pass new viewport size to active scene
		m_scene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		bool isTrue = true;
		ImGui::Begin("Main Window", &isTrue, window_flags);

		ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("AEngineDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Load Scene"))
				{
					// load scene
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SceneManager::SaveActiveToFile("serialized.scene");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ShowGameViewPort();
		ShowHierarchy();
		ShowInspector();
		ShowDebugWindow();

		ImGui::End();
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

	void Editor::ShowDebugWindow()
	{
		ImGui::Begin("Debug");

		ImGui::Text("Viewport hovered: %s", m_viewportHovered ? "true" : "false");
		ImGui::Text("Viewport focused: %s", m_viewportFocused ? "true" : "false");
		ImGui::Text("Cursor position: (%f, %f)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImGui::Text("Key down captured: %s", captureKeyDown ? "true" : "false");
		ImGui::Text("Key up captured: %s", captureKeyUp ? "true" : "false");
		ImGui::Text("Mouse click captured: %s", captureMouseClick ? "true" : "false");
		ImGui::Text("Mouse release captured: %s", captureMouseRelease ? "true" : "false");
		ImGui::Text("Mouse movement captured: %s", captureMouseMovement ? "true" : "false");

		ImGui::End();
	}

	void Editor::ShowGameViewPort()
	{
		ImGui::Begin("Game View");

		// check to see if the window is hovered
		m_viewportHovered |= ImGui::IsWindowHovered(ImGuiHoveredFlags_None);

		// capture available dimentions for game window
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_viewportSize = Math::vec2{
			static_cast<unsigned int>(viewportSize.x),
			static_cast<unsigned int>(viewportSize.y)
		};

		ImGui::Text("Viewport Size: %f, %f", viewportSize.x, viewportSize.y);

		// generate the framebuffer texture for mapping to the window

		// ImGui::Image(texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
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

		ImGui::Begin(std::string("Hierarchy - " + m_scene->GetIdent()).c_str());
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
		ShowTextComponent();
		ShowTerrainComponent();
		ShowSkyboxComponent();
		ShowWaterComponent();
		ShowCameraComponent();
		ShowScriptableComponent();
		ShowRigidBodyComponent();
		ShowBoxColliderComponent();
		ShowHeightMapColliderComponent();
		ShowPlayerControllerComponent();

		//show components as part of inspector
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
			ShowAddComponentPrompt<TagComponent>("Tag");
			ShowAddComponentPrompt<TransformComponent>("Transform");
			ShowAddComponentPrompt<RenderableComponent>("Renderable");
			ShowAddComponentPrompt<SkinnedRenderableComponent>("Skinned Renderable");
			ShowAddComponentPrompt<TextComponent>("Text");
			ShowAddComponentPrompt<TerrainComponent>("Terrain");
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

	template <typename T>
	void Editor::ShowRemoveButton()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 0.6f));
		if (ImGui::Button("Remove"))
		{
			m_selectedEntity.RemoveComponent<T>();
		}
		ImGui::PopStyleColor(3);
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
				ImGui::DragFloat3("Scale", &(scale->x), 1.0f, 0.0f, 0.0f, "%.3f");
			}
		}
	}

	void Editor::ShowRenderableComponent()
	{
		RenderableComponent* rc = m_selectedEntity.GetComponent<RenderableComponent>();
		if(rc != nullptr)
		{
			if(ImGui::CollapsingHeader("Renderable Component"))
			{
				ImGui::Checkbox("IsActive", &(rc->active));
				//do we need to know about the model and shader in here?

				ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("Remove").x - 1);
				ShowRemoveButton<RenderableComponent>();
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

	//didn't see in the scene? do we need in inspector?
	void Editor::ShowTextComponent()
	{
		TextComponent* tc = m_selectedEntity.GetComponent<TextComponent>();
		if(tc != nullptr)
		{
			if(ImGui::CollapsingHeader("Text Component"))
			{
				Math::vec2* position = &tc->position;
				ImGui::InputFloat2("Position", &(position->x));
				//float scale
				//vec3 colour
				//Font font
				//Shader shader
				//string text
			}
		}
	}

	void Editor::ShowTerrainComponent()
	{
		TerrainComponent* tc = m_selectedEntity.GetComponent<TerrainComponent>();
		if(tc != nullptr)
		{
			if(ImGui::CollapsingHeader("Terrain Component"))
			{
				ImGui::Checkbox("Is Actice", &(tc->active));
				//heightmap
				//shader
				//textures
				//yRange
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

	void Editor::ShowWaterComponent()
	{
		WaterComponent* wc = m_selectedEntity.GetComponent<WaterComponent>();
		if(wc != nullptr)
		{
			if(ImGui::CollapsingHeader("Water Component"))
			{
				ImGui::Checkbox("Is Active", &(wc->active));
				//water
				//shader
				//dudv
				//normal
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

	void Editor::ShowBoxColliderComponent()
	{
		BoxColliderComponent* bcc = m_selectedEntity.GetComponent<BoxColliderComponent>();
		if(bcc != nullptr)
		{
			if(ImGui::CollapsingHeader("Box Collider Component"))
			{
				ImGui::Checkbox("Is Trigger: ", &(bcc->isTrigger));
				Math::vec3* size = &bcc->size;
				ImGui::InputFloat3("Size", &(size->x), "%.3f");
			}
		}
	}

	void Editor::ShowHeightMapColliderComponent()
	{
		HeightMapColliderComponent* hcc = m_selectedEntity.GetComponent<HeightMapColliderComponent>();
		if(hcc != nullptr)
		{
			if(ImGui::CollapsingHeader("Heightmap Collider Component"))
			{
				ImGui::Checkbox("Is Trigger: ", &(hcc->isTrigger));
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
