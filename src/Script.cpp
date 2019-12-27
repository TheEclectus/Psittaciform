#include "Script.h"

Script::Script(const std::string& File) :
	_Name(File),
	_Description("A description would go here.")
{

}

Script::~Script()
{

}

const std::string& Script::Name() const
{
	return _Name;
}

const std::string& Script::Description() const
{
	return _Description;
}
