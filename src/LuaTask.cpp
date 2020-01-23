#include "LuaTask.h"

#define hasflag(var, enm) ((var & static_cast<uint8_t>(enm)) == static_cast<uint8_t>(enm))

uint8_t LuaTask::_ReadConfig(const std::filesystem::path& RootPath)
{
	std::filesystem::path ConfigPath = RootPath / "config.lua";
	if(!std::filesystem::exists(ConfigPath) || !std::filesystem::is_regular_file(ConfigPath))
		return false;

	// Human-readable text only
	sol::protected_function_result ConfigRes = _State.do_file(ConfigPath.string(), sol::load_mode::text);
	if (!ConfigRes.valid())
		return 0-1;

	sol::table ConfigTable = ConfigRes;
	if (!ConfigTable.valid())
		return 0-1;

	uint8_t RequestedPerms = 0u;
	for (auto CurKey : ConfigTable)
	{
		std::string Val = CurKey.second.as<std::string>();
		if (Val == "FileIO_Read")
			RequestedPerms |= static_cast<uint8_t>(Perms::FileIO_Read);
		else if (Val == "FileIO_ReadWrite")
			RequestedPerms |= static_cast<uint8_t>(Perms::FileIO_ReadWrite);
		else if (Val == "FileIO_ExternalFiles")
			RequestedPerms |= static_cast<uint8_t>(Perms::FileIO_ExternalFiles);

		else if (Val == "Exec_CLinkage")
			RequestedPerms |= static_cast<uint8_t>(Perms::Exec_CLinkage);
		else if (Val == "Exec_Shell")
			RequestedPerms |= static_cast<uint8_t>(Perms::Exec_Shell);
	}

	return RequestedPerms;
}

void LuaTask::_SetupState(uint8_t Perms)
{
	sol::state* State = &_State;
	//_Sandbox = new sol::environment(_State);
	sol::environment& Env = _Sandbox;

	Env = _State.globals();

	// MODIFIED LUA LIBRARY DEFINITIONS
	Env["dofile"]	= sol::nil;
	Env["load"]		= sol::nil;
	Env["loadfile"] = sol::nil;
	Env["print"]	= _State.globals()["print"]; // Will eventually print to imgui console

	Env["package.cpath"]	= "";
	Env["package.loadlib"]	= sol::nil;
	Env["package.path"]		= "";
	Env["require"]			= sol::nil;

	Env["io.close"]		= sol::nil;
	Env["io.input"]		= [State](const std::string& Path) {
		sol::safe_function IO_Input = State->globals()["io.input"];
		return IO_Input(Path);
	};
	Env["io.lines"]		= sol::nil;
	Env["io.open"]		= sol::nil;
	Env["io.output"]	= [State](const std::string& Path) {
		State->globals()["io.output"](Path);
	};
	Env["io.read"]		= [State](const sol::variadic_args Args) {
		// eventually read from imgui input
	};
	Env["io.write"]		= [State](const sol::variadic_args Args) {
		// eventually write to imgui terminal
	};

	// BITS +0-2 -- FILE PERMISSIONS

	if (hasflag(Perms, Perms::FileIO_Read))
	{
		Env["io.open"]		= [State, Perms](const std::string& Filename, sol::object Mode) {
			std::filesystem::path OpenPath = std::filesystem::absolute(Filename);
			/*if (!hasflag(Perms, Perms::FileIO_ExternalFiles))
			{
				if (OpenPath.string().find(std::filesystem::current_path().string()) != 0)
				{
					luaL_error("Insufficient permissions to call io.open on any file outside the task's working directory.")
				}
			}

			static std::string PrependText = (hasflag(Perms, Perms::FileIO_ExternalFiles) ?)*/
		};


		if (hasflag(Perms, Perms::FileIO_ReadWrite))
		{
			//Func()
		}
		else
		{
			// "Open" only works when Mode == "r"
			Env["io.open"] = [State](const std::string& Filename, sol::object Mode) {
				if (Mode.valid() && Mode.is<std::string>())
				{
					if (Mode.as<std::string>() != "r")
					{
						luaL_error(*State, "Insufficient permissions to call io.open called with any argument except 'r'.");
						return;
					}

					State->globals()["io.open"](Filename, "r");
				}
				else
				{
					State->globals()["io.open"](Filename);
				}
			};
		}

		if (hasflag(Perms, Perms::FileIO_ExternalFiles) && hasflag(Perms, Perms::FileIO_ReadWrite))
		{
			// External file RW perms
			Env["io.open"]		= _State.globals()["io.open"];
			Env["package.path"] = _State.globals()["package.path"];
		}
		else //if ()
		{

		}

		
	}
	
}

LuaTask::LuaTask(const std::filesystem::path& RootPath) :
	_State(),
	_Sandbox()
{
	uint8_t RequestedPerms = _ReadConfig(RootPath);

	//_RootState->open_libraries(sol::lib::base, sol::lib::string, sol::lib::ffi);
	//sol::lib::
}