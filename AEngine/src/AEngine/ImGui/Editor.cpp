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
	void Editor::Init(Window *window, const EditorProperties& props)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();

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

		/// \todo hook into event handling loop
		window->RegisterEventHandler<KeyPressed>(0, [](KeyPressed& e) -> bool {
			return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<KeyReleased>(0, [](KeyReleased& e) -> bool {
			return ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse;
		});

		window->RegisterEventHandler<MouseButtonPressed>(0, [](MouseButtonPressed& e) -> bool {
			return ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseButtonReleased>(0, [](MouseButtonReleased& e) -> bool {
			return ImGui::GetIO().WantCaptureMouse;
		});
		window->RegisterEventHandler<MouseMoved>(0, [](MouseMoved& e) -> bool {
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
		/// \todo think about implementing observer pattern
		m_scene = SceneManager::GetActiveScene();

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		//ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

		ShowGameViewPort();
		ShowHierarchy();
		ShowInspector();

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

	void Editor::ShowGameViewPort()
	{
		ImGui::Begin("Game View");
			//generate the framebuffer texture for mapping to the window
			//ImGui::Image(texture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	void Editor::ShowHierarchy()
	{
		std::vector<Uint16> entityids;
		m_scene->GetEntityIds(entityids);

		ImGui::Begin("Hierarchy");

		for(int i = 0; i < entityids.size(); i++)
		{
			Entity entity = m_scene->GetEntity(entityids[i]);
			std::string entityName = entity.GetComponent<TagComponent>()->tag;
			if(ImGui::Button(entityName.c_str()))
			{
				m_selectedEntity = m_scene->GetEntity(entityids[i]);
			}
		}
		ImGui::End();
	}
	
	void Editor::ShowInspector()
	{
		if(!m_selectedEntity.IsValid())
			return;
			
		ImGui::Begin("Inspector");

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
		ShowPhysicsHandle();
		ShowRigidBodyComponent();
		ShowBoxColliderComponent();
		ShowHeightMapColliderComponent();
		ShowPlayerControllerComponent();
		//show components as part of inspector

		//might need to have similar function for each entity and check against entity to view everything
		//build out for all components

		ImGui::End();
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
				ImGui::SliderFloat3("Translation", &(translation->x), -100.0f, 100.0f, "%.3f");
				
				Math::quat* orientation = &tc->orientation;
        		Math::vec3 eulerAnglesDegrees = Math::degrees(Math::eulerAngles(*orientation));
        		ImGui::DragFloat3("Rotation", &eulerAnglesDegrees.x, 1.0f, -180.0f, 180.0f, "%.3f");
        		*orientation = Math::quat(Math::radians(eulerAnglesDegrees));
				
				Math::vec3* scale = &tc->scale;
				ImGui::SliderFloat3("Scale", &(scale->x), 0, 100, "%.3f");
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

	void Editor::ShowPhysicsHandle()
	{
		PhysicsHandle* ph = m_selectedEntity.GetComponent<PhysicsHandle>();
		if(ph != nullptr)
		{
			if(ImGui::CollapsingHeader("Physics Handle"))
			{
				//physics stuff?
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
