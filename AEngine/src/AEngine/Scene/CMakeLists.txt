cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               Components.h
               DebugCamera.cpp
               DebugCamera.h
               Entity.cpp
               Entity.h
               Scene.cpp
               Scene.h
               SceneSerialiser.cpp
               SceneSerialiser.h
)
