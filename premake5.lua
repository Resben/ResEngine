--[[]
	author: Christien Alden (34119981)
	purpose: Build script for ACHO Engine workspace
--]]

-- directories
exedir = "executable/"
outputdir = "%{cfg.buildcfg}/"
projdir = "%{prj.name}/"

----------------------------------------------------------------------------------------------------
workspace "ACHO Engine Development"
	location "build"
	architecture "x64"
	startproject "AEngine-Editor"
	targetdir (exedir .. outputdir .. projdir)
	objdir ("%{cfg.location}/" .. outputdir .. projdir)

	configurations {
		"Debug",
		"Release"
	}

	group "Core"
		include "AEngine"
		-- include "AEngine-Editor"
	group ""

	group "Dependencies"
		include "AEngine/dependencies.lua"
	group ""
