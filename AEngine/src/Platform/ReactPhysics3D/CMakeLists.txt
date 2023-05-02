cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ReactCollider.cpp
               ReactCollider.h
               ReactCollisionBody.cpp
               ReactCollisionBody.h
               ReactPhysics.cpp
               ReactPhysics.h
               ReactRaycaster.cpp
               ReactRaycaster.h
)
