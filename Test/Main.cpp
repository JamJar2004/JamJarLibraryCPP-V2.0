#include <JamJar/Core.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

ExitStatus Start()
{
	ArrayList<String> list;
	list.Add("ABC");
	list.Add("DEF");
	list.Add("GHI");
	list.Add("JKL");
	list.Add("MNO");
	list.Add("PQR");

	Console::PrintLine(list);

	return ExitStatus::OK;
}