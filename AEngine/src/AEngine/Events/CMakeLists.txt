cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
target_sources(AEngine-Lib PRIVATE
               ApplicationEvent.h
               Event.h
               EventQueue.h
               EventQueue.cpp
               KeyEvent.h
               MouseEvent.h
)
