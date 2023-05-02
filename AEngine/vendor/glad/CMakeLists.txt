cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(glad
        DESCRIPTION "Glad library for OpenGL"
        LANGUAGES C)

add_library(glad STATIC)
target_include_directories(glad PUBLIC
                           include
)

add_subdirectory(src)
