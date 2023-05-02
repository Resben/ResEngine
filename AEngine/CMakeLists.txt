cmake_minimum_required(VERSION 3.0.0)
project(AEngine-Lib
        DESCRIPTION "Core static library for AEngine"
        VERSION 0.1.0
        LANGUAGES CXX
)

# global settings for project
set(BUILD_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS OFF)

# generate project
add_library(AEngine-Lib STATIC)
set_target_properties(AEngine-Lib PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(AEngine-Lib
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src
                           PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vendor
)

# parse project directory hierarchy
add_subdirectory(src)
add_subdirectory(vendor)

# link libraries
target_link_libraries(AEngine-Lib PRIVATE assimp)
target_link_libraries(AEngine-Lib PRIVATE EnTT::EnTT)
target_link_libraries(AEngine-Lib PRIVATE glad)
target_link_libraries(AEngine-Lib PRIVATE glm::glm)
target_link_libraries(AEngine-Lib PRIVATE glfw)
target_link_libraries(AEngine-Lib PRIVATE lua)
target_link_libraries(AEngine-Lib PRIVATE ReactPhysics3D::reactphysics3d)
target_link_libraries(AEngine-Lib PRIVATE sol2::sol2)
target_link_libraries(AEngine-Lib PRIVATE spdlog::spdlog_header_only)
target_link_libraries(AEngine-Lib PRIVATE stb_header_only)
target_link_libraries(AEngine-Lib PRIVATE yaml-cpp::yaml-cpp)
