cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/Components.h
               ${CMAKE_CURRENT_LIST_DIR}/DebugCamera.cpp
               ${CMAKE_CURRENT_LIST_DIR}/DebugCamera.h
               ${CMAKE_CURRENT_LIST_DIR}/Entity.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Entity.h
               ${CMAKE_CURRENT_LIST_DIR}/Scene.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Scene.h
               ${CMAKE_CURRENT_LIST_DIR}/SceneSerialiser.cpp
               ${CMAKE_CURRENT_LIST_DIR}/SceneSerialiser.h
)
