cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
add_executable(AEngine-Demo)
target_link_directories(AEngine-Demo PRIVATE
                        ${CMAKE_SOURCE_DIR}/AEngine
)

add_subdirectory(src)
target_link_libraries(AEngine-Demo PRIVATE
                      AEngine-Lib
)

set_property(TARGET AEngine-Demo
             PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_options(AEngine-Lib
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>: /W4>
)
