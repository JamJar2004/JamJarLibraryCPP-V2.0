#include <JamJar/Numerics.hpp>

#include <iostream>

template<FloatingPoint T>
T Add(T left, T right)
{
	return T::Maximum;
}

int main()
{
	Float32 i = Float32::Maximum;

	std::cout << std::is_integral_v<double> << std::endl;

	return 0;
}