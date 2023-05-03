# project setup
cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(
	AEngine-Demo
	DESCRIPTION "Demo app for AEngine"
	LANGUAGES CXX
)

# executable setup
add_executable(AEngine-Demo)
set_target_properties(
	AEngine-Demo PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/executable/$<CONFIG>
)

target_compile_options(
	AEngine-Lib PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>: /W4>
)

add_custom_command(
	TARGET AEngine-Demo
	POST_BUILD
	COMMAND
	${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_CURRENT_LIST_DIR}/assets" "${CMAKE_SOURCE_DIR}/executable/$<CONFIG>/assets"
)

# parse project directory hierarchy
add_subdirectory(src)

# link libraries
target_link_directories(
	AEngine-Demo PRIVATE
	${CMAKE_SOURCE_DIR}/AEngine
)

target_link_libraries(
	AEngine-Demo PRIVATE
	AEngine-Lib
)
