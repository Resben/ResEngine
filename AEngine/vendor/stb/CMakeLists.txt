cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(
	stb
	DESCRIPTION "stb_image header-only library"
	LANGUAGES C
)

add_library(stb_header_only INTERFACE)
target_include_directories(
	stb_header_only INTERFACE
	include
)
