# project setup
cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(
	AEngine-Lib
	DESCRIPTION "Core static library for AEngine"
	VERSION 0.1.0
	LANGUAGES CXX
)

# global project config
set(BUILD_SHARED_LIBS OFF)

# library setup
add_library(AEngine-Lib STATIC)
set_target_properties(
	AEngine-Lib PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(
	AEngine-Lib
	PRIVATE
		GLFW_INCLUDE_NONE
		AE_PHYSICS_REACT
	PUBLIC
		$<$<CONFIG:Debug>:AE_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:AE_DEBUG>
		$<$<BOOL:${WIN32}>:AE_PLATFORM_WINDOWS>
)

target_compile_options(
	AEngine-Lib
	PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>: /W4> # warning level 4
		$<$<CXX_COMPILER_ID:MSVC>: /wd4100 /wd4505> # disable warnings unused parameter (4100) and unreferenced local function (4505)
		$<$<CXX_COMPILER_ID:MSVC>: /external:anglebrackets /external:W0> # disable warnings from external headers
		$<$<CXX_COMPILER_ID:MSVC>: /bigobj>
)

target_include_directories(
	AEngine-Lib
	PUBLIC
		src
	PRIVATE
		vendor
)

# parse project directory hierarchy
add_subdirectory(src)
add_subdirectory(vendor)

# build tests for AEngine-Lib
if(AE_BUILD_TESTS)
	enable_testing()

	# test setup
	add_executable(AEngine-Test)
	add_subdirectory(test)

	# expose private headers for testing
	target_include_directories(
		AEngine-Test
		PRIVATE
			test
			AEngine/src
	)

	# link libraries
	target_link_libraries(AEngine-Test PRIVATE Catch2::Catch2WithMain)
	target_link_libraries(AEngine-Test PRIVATE AEngine-Lib)

	# expose to CTest
	add_test(NAME AEngine-Test COMMAND AEngine-Test)
endif()

# link private libraries
target_link_libraries(AEngine-Lib PRIVATE freetype)
target_link_libraries(AEngine-Lib PRIVATE glad)
target_link_libraries(AEngine-Lib PRIVATE glfw)
target_link_libraries(AEngine-Lib PRIVATE ReactPhysics3D::reactphysics3d)
target_link_libraries(AEngine-Lib PRIVATE stb_header_only)

# link public libraries
target_link_libraries(AEngine-Lib PUBLIC assimp)
target_link_libraries(AEngine-Lib PUBLIC EnTT::EnTT)
target_link_libraries(AEngine-Lib PUBLIC glm::glm)
target_link_libraries(AEngine-Lib PUBLIC lua)
target_link_libraries(AEngine-Lib PUBLIC sol2::sol2)
target_link_libraries(AEngine-Lib PUBLIC spdlog::spdlog_header_only)
target_link_libraries(AEngine-Lib PUBLIC yaml-cpp::yaml-cpp)
