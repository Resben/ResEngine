cmake_minimum_required(VERSION 3.0.0)
project("AEngine-Development" VERSION 0.1.0 LANGUAGES CXX)
add_subdirectory(AEngine)
add_subdirectory(AEngine-Demo)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             PROPERTY VS_STARTUP_PROJECT AEngine-Demo
)
