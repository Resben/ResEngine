#include "Editor.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "AEngine/Scene/Entity.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/SceneManager.h"

namespace AEngine
{
	void AEngine::Editor::Init(Window *window, const EditorProperties& props)
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
	}

	void AEngine::Editor::CreateNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void AEngine::Editor::Update()
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		static float f = 0.0f;
		static int counter = 0;

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

	void AEngine::Editor::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO &io = ImGui::GetIO();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//get our current window?
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			//change back to our window rendering context?
		}
	}

	void AEngine::Editor::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	

	//can be used to prevent the update of mouse if imgui needs the mouse same for keyboard method below
	bool Editor::WantCaptureMouse()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}
	
	bool Editor::WantCaptureKeyboard()
	{
		return ImGui::GetIO().WantCaptureKeyboard;
	}
}
