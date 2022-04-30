#include <JamJar/Numerics.hpp>

#include <JamJar/Data/Memory/Refs.hpp>

#include <iostream>


int main()
{
	SInt64 i = 0;
	SInt64 j = 0;

	std::cout << (j + i).ToRawValue();

	return 0;
}