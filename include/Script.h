#include <string>

class Script
{
private:
	const std::string _Name;
	const std::string _Description;

public:
	Script(const std::string& File);
	~Script();

	const std::string& Name() const;
	const std::string& Description() const;
};