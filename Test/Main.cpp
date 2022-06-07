#include <JamJar/Core.hpp>

#include <JamJar/Data/Reflection.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

#include <JamJar/Data/Collections/StringBuilder.hpp>

ExitStatus Start()
{
	/*String sentence = "He was in the forest looking to see the trees but none were there.";

	for(const String& word : sentence.Split(" "))
		Console::PrintLine(word);*/

	ArrayList<UInt32> list;
	list.Add(1U);
	list.Add(2U);
	list.AddRange(StackArray<UInt32, 3U>(3U, 4U, 5U));
	list.Add(8U);

	Console::PrintLine(list);

	list.Insert(5U, 6U);
	list.Insert(6U, 7U);

	Console::PrintLine(list);

	return ExitStatus::OK;
}