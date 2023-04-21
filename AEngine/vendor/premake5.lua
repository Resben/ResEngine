
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
	os.execute("cmake -G \"Visual Studio 16 2019\" -S glfw -B glfw")
	location "glfw/src"
	uuid (os.uuid("glfw"))
	kind "StaticLib"

--------------------------------------------------------------------------------
externalproject "assimp"
    os.execute("cmake -S assimp -G \"Visual Studio 16 2019\" -B assimp -D BUILD_SHARED_LIBS=OFF -D ASSIMP_BUILD_ZLIB=ON")
    location "assimp/code"
    uuid (os.uuid("assimp"))
    kind "StaticLib"
    dependson "zlibstatic"

externalproject "zlibstatic"
    location "assimp/contrib/zlib"
    uuid (os.uuid("zlibstatic"))
    kind "StaticLib"
	
--------------------------------------------------------------------------------
project "lua-lib"
	kind "StaticLib"
	staticruntime "Off"
	language "C"
	targetname "lua54"

	files {
		"lua/src/lapi.c",
		"lua/src/lcode.c",
		"lua/src/lctype.c",
		"lua/src/ldebug.c",
		"lua/src/ldo.c",
		"lua/src/ldump.c",
		"lua/src/lfunc.c",
		"lua/src/lgc.c",
		"lua/src/llex.c",
		"lua/src/lmem.c",
		"lua/src/lobject.c",
		"lua/src/lopcodes.c",
		"lua/src/lparser.c",
		"lua/src/lstate.c",
		"lua/src/lstring.c",
		"lua/src/ltable.c",
		"lua/src/ltm.c",
		"lua/src/lundump.c",
		"lua/src/lvm.c",
		"lua/src/lzio.c",
		"lua/src/lauxlib.c",
		"lua/src/lbaselib.c",
		"lua/src/lcorolib.c",
		"lua/src/ldblib.c",
		"lua/src/liolib.c",
		"lua/src/lmathlib.c",
		"lua/src/loadlib.c",
		"lua/src/loslib.c",
		"lua/src/lstrlib.c",
		"lua/src/ltablib.c",
		"lua/src/lutf8lib.c",
		"lua/src/linit.c"
	}

	filter "system:windows"
		-- disable warnings as this is an external library
		warnings "Off"