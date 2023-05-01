cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
add_executable(AEngine-Demo)
target_link_directories(AEngine-Demo PRIVATE
                        ${CMAKE_SOURCE_DIR}/AEngine
)

add_subdirectory(src)
target_link_libraries(AEngine-Demo PRIVATE
                      AEngine-Lib
)
