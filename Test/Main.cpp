#include <JamJar/Core.hpp>

#include <JamJar/Delegate.hpp>

#include <vector>

#include <JamJar/Data/Memory/Array.hpp>


ExitStatus Start()
{
	StackArray<UInt32, 11> array(1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U);
	Console::PrintLine(array);

	return ExitStatus::OK;
}