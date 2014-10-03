-- A solution contains projects, and defines the available configurations
solution "MyGameSever"
configurations { "Debug", "Release" }

-- A project defines one build target
project "MyGameServer"
	kind "ConsoleApp"
	language "C++"
	files { "**.h", "**.cpp" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }    
