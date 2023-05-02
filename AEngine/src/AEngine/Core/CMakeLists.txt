cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

target_sources(AEngine-Lib PRIVATE
               Application.cpp
               Application.h
               EntryPoint.cpp
               Identifier.cpp
               Identifier.h
               Input.h
               KeyCodes.h
               Layer.cpp
               Layer.h
               LayerStack.cpp
               LayerStack.h
               Logger.cpp
               Logger.h
               PerspectiveCamera.cpp
               PerspectiveCamera.h
               Timer.cpp
               Timer.h
               TimeStep.cpp
               TimeStep.h
               Window.cpp
               Window.h
)
