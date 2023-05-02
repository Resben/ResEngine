cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
add_executable(AEngine-Demo)
target_link_directories(AEngine-Demo PRIVATE
                        ${CMAKE_SOURCE_DIR}/AEngine
)

add_subdirectory(src)
target_link_libraries(AEngine-Demo PRIVATE
                      AEngine-Lib
)

set_target_properties(
    AEngine-Demo PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/executable/$<CONFIG>
)

target_compile_options(AEngine-Lib
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

add_custom_command(TARGET AEngine-Demo POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_SOURCE_DIR}/executable/$<CONFIG>/assets"
)
