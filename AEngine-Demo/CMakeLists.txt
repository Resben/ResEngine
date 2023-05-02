cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
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

add_custom_command(TARGET AEngine-Demo POST_BUILD
                   COMMENT "Copying AEngine-Demo to executable directory"

                   COMMAND ${CMAKE_COMMAND} -E copy
                   "$<TARGET_FILE:AEngine-Demo>"
                   "${CMAKE_SOURCE_DIR}/executable/$<CONFIGURATION>/$<TARGET_FILE_NAME:AEngine-Demo>"

                   # COMMAND ${CMAKE_COMMAND} -E copy-directory
                   # "${CMAKE_CURRENT_LIST_DIR}/assets"
                   # "${CMAKE_SOURCE_DIR}/executable/$<CONFIGURATION>/assets"
)
