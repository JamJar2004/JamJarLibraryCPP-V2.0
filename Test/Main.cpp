#include <JamJar/Numerics.hpp>

#include <iostream>

template<FloatingPoint T>
T Add(T left, T right)
{
	return T::Maximum;
}

int main()
{
	SInt64 a;
	SInt16 b = SInt16(a);
	UInt32 c = UInt32(b);

	Float32 f = c;

	return 0;
}