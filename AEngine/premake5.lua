project "AEngine"
	kind "StaticLib"
	runtime "Release"
	language "C++"
	-- todo: remove this
	libdirs "lib"

	defines {
      "GLFW_INCLUDE_NONE"
	}

	files {
		"AEngine.h",
		"src/**"
	}

	links {
		"glfw3.lib",
		"msvcrt.lib",
		"opengl32.lib",
		"glad"
	}

	includedirs {
		".",
		-- todo: don't include whole directory
		"vendor"
	}

   filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"
      warnings "Extra"
      disablewarnings {"4100", "4505"}
      -- disable warnings on vendor files
      externalincludedirs "vendor/**"
      externalwarnings "Default"

   filter "configurations:Debug"
      defines {"DEBUG", "AE_DEBUG"}
      symbols "On"

   filter "configurations:Release"
      defines "DNDEBUG"
      optimize "On"
