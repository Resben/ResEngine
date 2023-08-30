/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Builds the backends for ImGui.
 * \todo Potentially look at a way to move glfw3 and OpenGL to a deeper layer.
 *
 * \details
 * By including the backends here, the files get access to the glfw3 library
 * and the OpenGL library.
*/

// Currently using OpenGL and GLFW3 backends.
#include <imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/backends/imgui_impl_glfw.cpp>
