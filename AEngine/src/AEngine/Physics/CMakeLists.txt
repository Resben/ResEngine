cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               Collider.h
               CollisionBody.h
               Physics.cpp
               Physics.h
               PlayerController.cpp
               PlayerController.h
               Raycaster.cpp
               Raycaster.h
)
