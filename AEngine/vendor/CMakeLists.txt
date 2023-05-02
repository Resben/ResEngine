cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# vendor cmake projects
add_subdirectory(assimp)
add_subdirectory(entt)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(lua)
add_subdirectory(reactphysics3d)

target_include_directories(AEngine-Lib PRIVATE
                           ${CMAKE_CURRENT_LIST_DIR}/sol/include/
                           ${CMAKE_CURRENT_LIST_DIR}/spdlog/include/
                           ${CMAKE_CURRENT_LIST_DIR}/stb/include/
)
