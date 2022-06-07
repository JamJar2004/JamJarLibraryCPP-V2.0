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

	void Print(const String& message) { Console::PrintLine(message); }
};

ExitStatus Start()
{
	Dynamic value1 = String("Hello");
	Dynamic value2 = 6U;

	value1 = String("World!");

	Console::PrintLine(value1 == value2);

	return ExitStatus::OK;
}