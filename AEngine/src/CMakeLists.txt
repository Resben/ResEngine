cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
add_subdirectory(AEngine)
add_subdirectory(Platform)

target_sources(AEngine-Lib PUBLIC
               AEngine.h
)
