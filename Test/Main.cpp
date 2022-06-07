#include <JamJar/Core.hpp>

#include <JamJar/Data/Reflection.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

#include <JamJar/Data/Collections/StringBuilder.hpp>
#include <JamJar/Dynamic.hpp>


class Object
{
public:
	Object()
	{
		Console::PrintLine("Created!");
	}

	Object(const Object& other)
	{
		Console::PrintLine("Copied!");
	}

	~Object()
	{
		Console::PrintLine("Destroyed!");
	}
};

ExitStatus Start()
{
	Dynamic value = String("Hello World!");

	Console::PrintLine(value);

	return ExitStatus::OK;
}