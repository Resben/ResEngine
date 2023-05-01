cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLGraphicsAPI.cpp
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLGraphicsAPI.h
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLGraphicsContext.cpp
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLGraphicsContext.h
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLMesh.cpp
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLMesh.h
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLShader.cpp
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLShader.h
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLTexture.cpp
               ${CMAKE_CURRENT_LIST_DIR}/OpenGLTexture.h
)
