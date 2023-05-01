cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/Script.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Script.h
               ${CMAKE_CURRENT_LIST_DIR}/ScriptState.cpp
               ${CMAKE_CURRENT_LIST_DIR}/ScriptState.h
)
