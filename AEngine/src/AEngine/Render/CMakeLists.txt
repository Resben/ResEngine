cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/GraphicsAPI.cpp
               ${CMAKE_CURRENT_LIST_DIR}/GraphicsAPI.h
               ${CMAKE_CURRENT_LIST_DIR}/GraphicsContext.cpp
               ${CMAKE_CURRENT_LIST_DIR}/GraphicsContext.h
               ${CMAKE_CURRENT_LIST_DIR}/HeightMap.cpp
               ${CMAKE_CURRENT_LIST_DIR}/HeightMap.h
               ${CMAKE_CURRENT_LIST_DIR}/Mesh.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Mesh.h
               ${CMAKE_CURRENT_LIST_DIR}/Model.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Model.h
               ${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Renderer.h
               ${CMAKE_CURRENT_LIST_DIR}/Shader.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Shader.h
               ${CMAKE_CURRENT_LIST_DIR}/Texture.cpp
               ${CMAKE_CURRENT_LIST_DIR}/Texture.h
)

