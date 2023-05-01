cmake_minimum_required(VERSION 3.0.0)
add_subdirectory(AEngine)
add_subdirectory(Platform)

target_sources(AEngine-Lib PUBLIC
               ${CMAKE_CURRENT_LIST_DIR}/AEngine.h
)
