cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# vendor cmake projects
add_subdirectory(assimp)
add_subdirectory(glfw)

target_include_directories(AEngine-Lib PRIVATE
                           ${CMAKE_CURRENT_LIST_DIR}/entt/include/
                           ${CMAKE_CURRENT_LIST_DIR}/glm/include/
                           ${CMAKE_CURRENT_LIST_DIR}/sol/include/
                           ${CMAKE_CURRENT_LIST_DIR}/spdlog/include/
                           ${CMAKE_CURRENT_LIST_DIR}/stb/include/
)
