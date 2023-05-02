cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
project(AEngine-Lib
        DESCRIPTION "Core static library for AEngine"
        VERSION 0.1.0
        LANGUAGES CXX
)

# generate project
add_library(AEngine-Lib STATIC)
set_target_properties(AEngine-Lib PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(AEngine-Lib
                           PUBLIC src
                           PRIVATE vendor
)

target_compile_definitions(AEngine-Lib
                           PRIVATE
                           GLFW_INCLUDE_NONE
                           AE_PHYSICS_REACT

                           PUBLIC
                           $<$<CONFIG:Debug>:AE_DEBUG>
                           $<$<CONFIG:RelWithDebInfo>:AE_DEBUG>
                           $<$<BOOL:${WIN32}>:AE_PLATFORM_WINDOWS>

)

target_compile_options(AEngine-Lib
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

set(BUILD_SHARED_LIBS OFF)

# parse project directory hierarchy
add_subdirectory(src)
add_subdirectory(vendor)

# link libraries
target_link_libraries(AEngine-Lib PRIVATE glad)
target_link_libraries(AEngine-Lib PRIVATE glfw)
target_link_libraries(AEngine-Lib PRIVATE ReactPhysics3D::reactphysics3d)
target_link_libraries(AEngine-Lib PRIVATE stb_header_only)

target_link_libraries(AEngine-Lib PUBLIC assimp)
target_link_libraries(AEngine-Lib PUBLIC EnTT::EnTT)
target_link_libraries(AEngine-Lib PUBLIC glm::glm)
target_link_libraries(AEngine-Lib PUBLIC lua)
target_link_libraries(AEngine-Lib PUBLIC sol2::sol2)
target_link_libraries(AEngine-Lib PUBLIC spdlog::spdlog_header_only)
target_link_libraries(AEngine-Lib PUBLIC yaml-cpp::yaml-cpp)
