# project-controlled libs
add_subdirectory(glad)
add_subdirectory(lua)
add_subdirectory(stb)

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
add_subdirectory(reactphysics3d)
add_subdirectory(spdlog)
add_subdirectory(yaml-cpp)
