cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/WindowsInput.cpp
               ${CMAKE_CURRENT_LIST_DIR}/WindowsInput.h
               ${CMAKE_CURRENT_LIST_DIR}/WindowsKeys.cpp
               ${CMAKE_CURRENT_LIST_DIR}/WindowsKeys.h
               ${CMAKE_CURRENT_LIST_DIR}/WindowsWindow.cpp
               ${CMAKE_CURRENT_LIST_DIR}/WindowsWindow.h
)
