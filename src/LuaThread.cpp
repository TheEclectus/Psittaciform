#include "LuaThread.h"

#include <Sol2/sol.hpp>

LuaThread::LuaThread() :
	_RootState(new sol::state())
{
	_RootState->open_libraries(sol::lib::base, sol::lib::string, sol::lib::ffi);
	sol::lib::
}