cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
cmake_policy(VERSION 3.25)
project("AEngine-Development"
        VERSION 0.1.0
        LANGUAGES CXX)

# set policies for project
set(CMAKE_POLICY_DEFAULT_CMP0076 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_subdirectory(AEngine)
add_subdirectory(AEngine-Demo)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             PROPERTY VS_STARTUP_PROJECT AEngine-Demo
)
