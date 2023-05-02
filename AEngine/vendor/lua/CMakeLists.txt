cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(lua-lib
        DESCRIPTION "Lua library"
        LANGUAGES C
       	)

add_library(lua STATIC)
target_include_directories(lua PUBLIC
                           src
)

add_subdirectory(src)
