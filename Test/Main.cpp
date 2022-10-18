#include <JamJar/Core.hpp>

#include <JamJar/Data/Reflection.hpp>

#include <JamJar/Dynamic.hpp>

ExitStatus Start()
{
	const TypeInfo& stringType = Reflect::GetType<MutableString>();

	HeapArray<MutableString> array = StackArray<MutableString, 5>("0", "1", "2", "3", "4");

	DynamicArray array2(array);
	array2[0U] = MutableString("ABC");
	array2[1U] = MutableString("DEF");
	array2[2U] = MutableString("GHI");
	array2[3U] = MutableString("JKL");
	array2[4U] = MutableString("MNO");

	for(Size i = 0U; i < array2.Count(); i++)
		Console::PrintLine(array2[i].Cast<MutableString>());

	//Console::PrintLine(array);

	return ExitStatus::OK;
}