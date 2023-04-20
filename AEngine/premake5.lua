--[[]
	author: Christien Alden (34119981)
	purpose: Build script for AEngine project
--]]

project "AEngine"
	kind "StaticLib"
	staticruntime "Off"
	language "C++"

	files {
		"src/**",
		"platform/**"
	}

	includedirs {
		"src",
		"vendor/entt/include",
		"vendor/glad/include",
		"vendor/glfw/include",
		"vendor/glm/include",
		"vendor/spdlog/include",
		"vendor/stb/include",
		"vendor/reactphysics3d/include"
	}

	dependson {
		"glad",
		"glfw",
		"assimp"
	}

	libdirs {
		"lib"
	}

	links {
		"opengl32.lib",
		"reactphysics3d.lib",
		"glfw",
		"glad",
		"assimp"
	}

	defines {
	  "GLFW_INCLUDE_NONE",
	  "AE_PHYSICS_REACT"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		warnings "Extra"
		defines {
			"AE_PLATFORM_WINDOWS"
		}
		disablewarnings {
			"4100",
			"4505"
		}
		-- disable warnings on vendor files
		externalincludedirs "vendor/**"
		externalwarnings "Default"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		links {
			"yaml-cpp_d.lib"
		}
		defines {
			"DEBUG",
			"AE_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		defines "DNDEBUG"
		optimize "On"
		links {
			"yaml-cpp.lib"
		}
