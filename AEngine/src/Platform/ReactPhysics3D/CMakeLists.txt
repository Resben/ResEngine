cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/ReactCollider.cpp
               ${CMAKE_CURRENT_LIST_DIR}/ReactCollider.h
               ${CMAKE_CURRENT_LIST_DIR}/ReactCollisionBody.cpp
               ${CMAKE_CURRENT_LIST_DIR}/ReactCollisionBody.h
               ${CMAKE_CURRENT_LIST_DIR}/ReactPhysics.cpp
               ${CMAKE_CURRENT_LIST_DIR}/ReactPhysics.h
               ${CMAKE_CURRENT_LIST_DIR}/ReactRaycaster.cpp
               ${CMAKE_CURRENT_LIST_DIR}/ReactRaycaster.h
)
