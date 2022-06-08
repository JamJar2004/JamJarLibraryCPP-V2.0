#include <JamJar/Core.hpp>

#include <JamJar/Data/Reflection.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

#include <JamJar/Data/Collections/StringBuilder.hpp>
#include <JamJar/Dynamic.hpp>

class Object
{
public:
	//Object(const Object&) = delete;

	//Object& operator=(const Object&) = delete;

	Object()
	{
		Console::PrintLine("Created!");
	}

	Object(const Object& other)
	{
		Console::PrintLine("Copied!");
	}

	Object(Object&& other) noexcept
	{
		Console::PrintLine("Moved!");
	}

	~Object()
	{
		Console::PrintLine("Destroyed!");
	}

	void Print(const String& message) { Console::PrintLine(message); }

	friend Boolean operator==(const Object& left, const Object& right) { return true;  }
	friend Boolean operator!=(const Object& left, const Object& right) { return false; }
};

ExitStatus Start()
{


	return ExitStatus::OK;
}