
project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	files {
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c"
	}

	includedirs {
		"glad/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--------------------------------------------------------------------------------
externalproject "glfw"
	os.execute("cmake -S glfw -B glfw")
	location "glfw/src"
	uuid (os.uuid("glfw"))
	kind "StaticLib"

--------------------------------------------------------------------------------
externalproject "assimp"
    os.execute("cmake -S assimp -B assimp -D BUILD_SHARED_LIBS=OFF -D ASSIMP_BUILD_ZLIB=ON")
    location "assimp/code"
    uuid (os.uuid("assimp"))
    kind "StaticLib"
    dependson "zlibstatic"

externalproject "zlibstatic"
    location "assimp/contrib/zlib"
    uuid (os.uuid("zlibstatic"))
    kind "StaticLib"