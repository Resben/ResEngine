cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/Collider.h
               ${CMAKE_CURRENT_LIST_DIR}/CollisionBody.h
               ${CMAKE_CURRENT_LIST_DIR}/Physics.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Physics.h
               ${CMAKE_CURRENT_LIST_DIR}/PlayerController.cpp
               ${CMAKE_CURRENT_LIST_DIR}/PlayerController.h
               ${CMAKE_CURRENT_LIST_DIR}/Raycaster.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Raycaster.h
)
