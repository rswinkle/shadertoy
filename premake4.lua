-- A solution contains projects, and defines the available configurations
solution "ShaderToy"
	configurations { "Debug", "Release" }
	location "build"

	-- A project defines one build target
	project "shadertoy"
		location "build"
		kind "ConsoleApp"
		language "C++"
		files
		{
		"main.cpp",
		"glcommon/rsw_math.cpp",
		"glcommon/rsw_math.h",
		"glcommon/glslprogram.cpp",
		"glcommon/glslprogram.h"
		}
		--excludes { }
		includedirs { "./glcommon" }
		--      libdirs { }
		links { "SDL2", "GLEW", "GL" } 
		targetdir "build"

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration { "linux", "gmake" }
			buildoptions { "-ansi", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-x c++" }
