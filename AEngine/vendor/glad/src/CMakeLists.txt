cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(glad PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/glad.c
)
