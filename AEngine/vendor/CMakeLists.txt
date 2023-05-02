cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# vendor cmake projects
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
add_subdirectory(assimp)

add_subdirectory(entt)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(lua)
add_subdirectory(reactphysics3d)
add_subdirectory(sol2)
add_subdirectory(spdlog)
add_subdirectory(stb)
add_subdirectory(yaml-cpp)
