#pragma once

#include <cstddef>
#include <string>

#include <Sol2/forward.hpp>

class LuaThread
{
public:
	enum class Perms
	{
		// LUA LIBRARIES
		/*
			Always enabled.
			Modified functions:
			- print()			-> calls con.print() instead.
			
		*/
		Basic		= 1,
		
		/*
			Always enabled.
			Modified functions:
			- require()			-> Return nil based on acquired permissions
		*/
		Package		= 2,

		// Always enabled.
		String		= 4,
		// Always enabled.
		Table		= 8,
		// Always enabled.
		Math		= 16,

		/*
			Big Flashy Warning.
			"The IO library may be used to read, write and execute 
			with current user permissions on the host system."

			Modified functions:
			io.close()			-> disabled
			io.input()			-> disabled
			io.lines()			-> disabled
			io.output()			-> disabled
			io.read()			-> disabled
		*/
		IO			= 32,
	};

private:
	sol::state *_RootState;		// The Lua state that the main program operates on.
	
public:
	LuaThread();

	//sol::environment 
};