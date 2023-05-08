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
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/executable/$<CONFIG>
)

target_compile_options(
	AEngine-Demo PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>: /W4> # warning level 4
	$<$<CXX_COMPILER_ID:MSVC>: /external:anglebrackets /external:W0> # disable warnings from external headers
)

add_custom_command(
	TARGET AEngine-Demo
	POST_BUILD
	COMMAND
	${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_CURRENT_LIST_DIR}/assets" "$<TARGET_FILE_DIR:AEngine-Demo>/assets"
	COMMAND_EXPAND_LISTS
	COMMENT "AEngine-Demo: Copying assets to executable directory"
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
