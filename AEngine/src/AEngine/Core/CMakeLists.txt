cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Application.h
               ${CMAKE_CURRENT_LIST_DIR}/EntryPoint.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Identifier.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Identifier.h
               ${CMAKE_CURRENT_LIST_DIR}/Input.h
               ${CMAKE_CURRENT_LIST_DIR}/KeyCodes.h
               ${CMAKE_CURRENT_LIST_DIR}/Layer.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Layer.h
               ${CMAKE_CURRENT_LIST_DIR}/LayerStack.cpp
               ${CMAKE_CURRENT_LIST_DIR}/LayerStack.h
               ${CMAKE_CURRENT_LIST_DIR}/Logger.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Logger.h
               ${CMAKE_CURRENT_LIST_DIR}/PerspectiveCamera.cpp
               ${CMAKE_CURRENT_LIST_DIR}/PerspectiveCamera.h
               ${CMAKE_CURRENT_LIST_DIR}/Timer.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Timer.h
               ${CMAKE_CURRENT_LIST_DIR}/TimeStep.cpp
               ${CMAKE_CURRENT_LIST_DIR}/TimeStep.h
               ${CMAKE_CURRENT_LIST_DIR}/Window.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Window.h
)
