# project-controlled libs
add_subdirectory(glad)
add_subdirectory(lua)
add_subdirectory(stb)

# catch2
if(AE_BUILD_TESTS)
	set(BUILD_TESTING OFF CACHE BOOL "Build Catch2 test binary")
	set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "Build Catch2 extra features")
	set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Install Catch2 documentation")
	add_subdirectory(Catch2)
endif()

# assimp
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
add_subdirectory(assimp)

# glfw
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# sol2
set(SOL2_ENABLE_INSTALL OFF)
add_subdirectory(sol2)

# default external libs
add_subdirectory(entt)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(ImGuizmo)
add_subdirectory(reactphysics3d)
add_subdirectory(spdlog)
add_subdirectory(yaml-cpp)
add_subdirectory(freetype)
