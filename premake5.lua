--[[]
	author: Christien Alden (34119981)
	purpose: Build script for ACHO Interactive projects
--]]

-- Directories
outputdir = "%{cfg.buildcfg}/"
projdir = "%{prj.name}/"

----------------------------------------------------------------------------------------------------
workspace "ACHO Interactive"
	location "visualStudio"
	debugdir "."
	architecture "x64"
	configurations {"Debug", "Release"}
	runtime "Release"
	startproject "AEngine Sandbox"
	defines "GLFW_INCLUDE_NONE"

	includedirs {"include", "vendor"}
	libdirs "lib"
	language "C++"
	targetdir ("executable/" .. outputdir .. projdir)
	objdir ("%{cfg.location}/build/" .. outputdir .. projdir)

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		warnings "Extra"
		disablewarnings {"4100", "4505"}
		externalincludedirs "vendor/**"
		externalwarnings "Default"

	filter "configurations:Debug"
		defines {"DEBUG", "AE_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		defines "DNDEBUG"
		optimize "On"

----------------------------------------------------------------------------------------------------
project "AEngine Sandbox"
	dependson "AEngine"
	kind "ConsoleApp"

	files "src/**"
	removefiles {
		"src/AEngine/**"
	}

	links {
		"AEngine"
	}

----------------------------------------------------------------------------------------------------
project "AEngine"
	kind "StaticLib"

	files{
		"include/AEngine/**.h",
		"src/AEngine/**"
	}

	links {
		"glfw3.lib",
		"msvcrt.lib",
		"opengl32.lib"
	}

----------------------------------------------------------------------------------------------------
