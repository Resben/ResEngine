cmake_minimum_required(VERSION 3.0.0)
project(AEngine-Lib
        DESCRIPTION "Core static library for AEngine"
        VERSION 0.1.0
        LANGUAGES CXX
)

add_library(AEngine-Lib STATIC)
set_target_properties(AEngine-Lib PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(AEngine-Lib
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src
                           PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vendor
)

add_subdirectory(src)
add_subdirectory(vendor)

target_link_libraries(AEngine-Lib PRIVATE glad)
target_link_libraries(AEngine-Lib PRIVATE glfw)
target_link_libraries(AEngine-Lib PRIVATE assimp)
target_link_libraries(AEngine-Lib PRIVATE EnTT::EnTT)
