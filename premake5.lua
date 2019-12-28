print("Making dirs")
-- Create /ext directories
os.mkdir("./ext")
os.mkdir("./ext/SDL2")
-- Create /license directory
os.mkdir("./license")

-- SDL2 paths
newoption {
	trigger =		"sdl2",
	description =	"SDL2 directory containing /include, /lib, and /README-SDL.txt",
	default =		"./ext/SDL2"
}
newoption {
	trigger =		"sdl2_include",
	description =	"SDL2 directory containing header files."
}
newoption {
	trigger =		"sdl2_lib",
	description =	"SDL2 directory containing .lib and .dll files."
}
newoption {
	trigger =		"sdl2_license",
	description =	"SDL2 directory containing README-SDL.txt"
}

if(not _OPTIONS["sdl2_include"]) then
	_OPTIONS["sdl2_include"] = _OPTIONS["sdl2"] .. "/include"
end
if(not os.isdir(_OPTIONS["sdl2_include"])) then 
	premake.error("[ERROR] SDL2 include directory '" .. _OPTIONS["sdl2_include"] .. " does not exist or is not a directory.") 
end

if(not _OPTIONS["sdl2_lib"]) then
	_OPTIONS["sdl2_lib"] = _OPTIONS["sdl2"] .. "/lib"
end
if(not os.isdir(_OPTIONS["sdl2_lib"])) then 
	premake.error("[ERROR] SDL2 lib directory '" .. _OPTIONS["sdl2_lib"] .. " does not exist or is not a directory.") 
end

if(not _OPTIONS["sdl2_license"]) then
	_OPTIONS["sdl2_license"] = _OPTIONS["sdl2"] .. "/README-SDL.txt"
end
if(not os.isfile(_OPTIONS["sdl2_license"])) then 
	premake.error("[ERROR] SDL2 license file '" .. _OPTIONS["sdl2_license"] .. " does not exist or is not a file.") 
else
	os.copyfile(_OPTIONS["sdl2_license"], "./license/README-SDL.txt")
end

workspace "Psittaciform"
	configurations { "Debug", "ReleaseDebug", "Release" }
	platforms {"x86", "x86_64", "ARM"}

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x86_64"
		architecture "x86_64"

	filter "platforms:ARM"
		architecture "ARM"

project "Psittaciform"
	kind "WindowedApp"
	language "C++"
	objdir "./obj/%{cfg.buildcfg}-%{cfg.architecture}"
	targetdir "./bin/%{cfg.buildcfg}-%{cfg.architecture}"	-- "/bin/Debug-x86", "/bin/Release-x86_64", etc.
	entrypoint "WinMainCRTStartup"
	
	-- Global settings
	staticruntime "Off"
	flags {
		"FatalWarnings",	-- Compiler and link warnings are treated as errors
		--"StaticRuntime",	--staticruntime "Off"	-- Will make Multi-Threaded (Debug) DLL
		--"WinMain",
	}
	
	files {
		"./deps/**.c",
		"./deps/**.cpp",
		"./deps/**.h",

		"./include/**.h",
		"./src/**.c",
		"./src/**.cpp",
	}

	-- Includes
	includedirs { 		-- For project-local headers ("" includes)
		"./include",
		"./deps",
	} 
	
	sysincludedirs {	-- For external dependencies (<> includes)
		_OPTIONS["sdl2_include"],
	}
	
	filter "system:windows"
		defines {
			"TRAY_WINAPI",
		}
	
	filter "action:vs*"
		defines {
			"WINDOWS_IGNORE_PACKING_MISMATCH",
		}
	
	-- Libraries and linking
	filter "platforms:x86"	
		syslibdirs {
			_OPTIONS["sdl2_lib"] .. "/x86",
		}
		links {
			"SDL2",
			"SDL2main"
		}

	filter "configurations:Debug"
		runtime "Debug"		-- Multithreaded Debug
		symbols "On"		-- Debugging symbols on
		optimize "Off"		-- Turn off optimizations.
