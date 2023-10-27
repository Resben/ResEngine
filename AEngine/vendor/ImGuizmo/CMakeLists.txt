cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(
	imguizmo
	DESCRIPTION "ImGuizmo library"
	LANGUAGES CXX
)

add_library(imguizmo STATIC)
target_include_directories(
	imguizmo PUBLIC
		${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
	imguizmo
		PRIVATE
	ImGuizmo.cpp
	ImGuizmo.h
)

target_link_libraries(imguizmo PRIVATE imgui)
