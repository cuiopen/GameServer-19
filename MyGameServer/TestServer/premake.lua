-- A solution contains projects, and defines the available configurations
solution "TestSever"
configurations { "Debug", "Release" }

-- A project defines one build target
project "TestServer"
	kind "ConsoleApp"
	language "C++"
	files { "**.h", "**.cpp" }

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }    
