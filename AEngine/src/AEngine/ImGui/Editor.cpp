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

		//need to change this probalby
		m_inspectorId = UINT16_MAX;
		//add scene member variable

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
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		static float f = 0.0f;
		static int counter = 0;

		CreateHierarchy();

		if(m_inspectorId != UINT16_MAX)
		{
			CreateInspector();
		}

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		Scene* scene = SceneManager::GetActiveScene();
		Entity entity = scene->GetEntity("testitems");
		TransformComponent* transform = entity.GetComponent<TransformComponent>();
		Math::vec3* translation = &transform->translation;

		ImGui::SliderFloat3("Translation", &(translation->x), -100.0f, 100.0f, "%.3f");
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		    counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
	
	
	void Editor::CreateHierarchy()
	{
		Scene* scene = SceneManager::GetActiveScene();
		std::vector<Uint16> entityids;
		scene->GetEntityIds(entityids);

		ImGui::Begin("Hierarchy");

		for(int i = 0; i < entityids.size(); i++)
		{
			Entity entity = scene->GetEntity(entityids[i]);
			std::string entityName = entity.GetComponent<TagComponent>()->tag;
			if(ImGui::Button(entityName.c_str()))
			{
				m_inspectorId = entityids[i];
			}
		}
		ImGui::End();
	}
	
	void Editor::CreateInspector()
	{
		ImGui::Begin("Inspector");

		CreateTagComponent();
		CreateTransformComponent();
		CreateRenderableComponent();
		//show components as part of inspector

		//might need to have similar function for each entity and check against entity to view everything
		//build out for all components

		ImGui::End();
	}

	void Editor::CreateTagComponent()
	{
		Scene* scene =  SceneManager::GetActiveScene();
		Entity entity = scene->GetEntity(m_inspectorId);
		TagComponent* tc = entity.GetComponent<TagComponent>();
		if(tc != nullptr)
		{
			ImGui::Text("Name: %s", tc->tag.c_str());
			ImGui::Text("ID: %d", tc->ident);
		}
	}
	
	void Editor::CreateTransformComponent()
	{
		Scene* scene = SceneManager::GetActiveScene();
		Entity entity = scene->GetEntity(m_inspectorId);
		TransformComponent* tc = entity.GetComponent<TransformComponent>();
		if(tc != nullptr)
		{
			if(ImGui::CollapsingHeader("Transform Component"))
			{
				Math::vec3* translation =  &tc->translation;
				ImGui::SliderFloat3("Translation", &(translation->x), -100.0f, 100.0f, "%.3f");
				//Need to change convert to degrees and vector3?
				Math::quat* orientation = &tc->orientation;
				ImGui::SliderFloat4("Orientation", &(orientation->x), -180, 180, "%.3f");
				Math::vec3* scale = &tc->scale;
				ImGui::SliderFloat3("Scale", &(scale->x), -100, 100, "%.3f");
			}
		}
	}
	
	void Editor::CreateRenderableComponent()
	{
		Scene* scene = SceneManager::GetActiveScene();
		Entity entity = scene->GetEntity(m_inspectorId);
		RenderableComponent* rc = entity.GetComponent<RenderableComponent>();
		if(rc != nullptr)
		{
			if(ImGui::CollapsingHeader("Renderable Component"))
			{
				ImGui::Checkbox("IsActive", &(rc->active));
				//do we need to know about the model and shader in here?
			}
		}
	}
}
