#include <JamJar/Core.hpp>

#include <JamJar/Data/Reflection.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

#include <JamJar/Data/Collections/StringBuilder.hpp>

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
	String sentence = "    He was in the forest looking to see the trees but none were there.";
	Console::PrintLine(sentence.Trim());

	//ArrayList<String> list;
	//list.Add("A");
	//list.Add("B");
	//list.Add("C");
	//list.Add("D");
	//list.Add("E");
	//list.Add("F");
	//list.Add("G");
	//list.Add("H");
	//list.Add("I");

	//Console::PrintLine(list.ToArray());

	return ExitStatus::OK;
}