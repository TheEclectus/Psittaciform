#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <string>

#include <Sol2/sol.hpp>

class LuaTask
{
public:
	/*
		MODIFIED LUA LIBRARY DEFINITIONS

		Base:
		- dofile()				-> disabled by default, altered to fail if not called with an argument
		- load()				-> disabled by default
		- loadfile()			-> disabled by default, altered to fail it not called with an argument
		- print()				-> calls con.print() instead.

		Package:
		- package.cpath			-> set to "" by default
		- package.loadlib()		-> disabled by default
		- package.path			-> set to "" by default
		- require()				-> disabled by default

		File I/O:
		- io.close()			-> disabled (don't want default output closed)
		- io.input()			-> altered to fail if called with any arguments
		- io.lines()			-> disabled
		- io.open()				-> disabled by default
		- io.output()			-> altered to fail if called with any arguments
		- io.read()				-> instead of calling io.input():read(), calls con.readLine() instead.
		- io.write()			-> instead of calling io.output():write(), calls con.print() instead.
	*/
	enum class Perms : uint8_t
	{
		/*
			Key:
			! - Potential for abuse or damage
			!! - High potential for abuse or damage
		*/

		// BITS +0-2 -- FILE PERMISSIONS
		/*
			FileIO required for bits 2 and 3 to have function.

			Allows for the loading of files through require(), io.*, etc.

			Modified members:
				(FileIO_Read):
				- io.open()			-> enabled IF AND ONLY IF mode is set to "r"

				(FileIO_ReadWrite):
				- io.open()			-> enabled completely

				(FileIO_Read*):
				- io.open()			-> can only read from files in program directory (prepend path with abs path ending in path separator)
				- package.path		-> set to program's /deps directory
				- require()			-> becomes enabled, with restrictions set by paths

				(FileIO_ExternalFiles):
				- io.open()			-> can read anywhere on system
				- package.path		-> exposed to script as variable
				
		*/
		FileIO_Read				= 1 << 0,				// Files may be read.
		FileIO_ReadWrite		= 2 << 0 | FileIO_Read,	// ! Files may be read, written to, and created.
		FileIO_ExternalFiles	= 4 << 0,				// ! File perms on files outside program directory.

		// BITS +3-5 -- EXECUTION PERMISSIONS
		Exec_CLinkage			= 1 << 3,				// !! C libraries may linked to and executed.
		Exec_Shell				= 2 << 3,				// !! Shell commands and external programs may be run.

		/*
			Big Flashy Warning.
			"The IO library may be used to read, write and execute 
			with current user permissions on the host system."

			Modified members:
			
		*/

		/*
			Big Flashy Warning.
			"The OS library may be used to access environment variables,
			and to execute arbitrary shell commands with current user 
			permissions on the host system."

			Modified members:
			
		*/
	};

	enum class PermWarnings
	{

	};

private:
	sol::state _State;		// The Lua state that the task operates on.
	sol::environment _Sandbox;
	sol::safe_function Func;
	
	// Returns -1 if permissions are denied or the path doesn't exist (FOR NOW!!!)
	uint8_t _ReadConfig(const std::filesystem::path& RootPath);
	void _SetupState(uint8_t Perms);
	
public:
	LuaTask(const std::filesystem::path& RootPath);

	//sol::environment 
};