cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(
	"AEngine-Development"
	VERSION 0.1.0
	LANGUAGES CXX
)

# set cmake policies
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# set options
option(AE_BUILD_TESTS "Build tests" ON)

if(AE_BUILD_TESTS)
	# expose workspace to CTest
	enable_testing()
endif()

add_subdirectory(AEngine)
add_subdirectory(AEngine-Demo)

set_property(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	PROPERTY VS_STARTUP_PROJECT AEngine-Demo
)
