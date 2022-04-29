#pragma once

#include "Concepts.hpp"

class String
{

};

template<typename T>
concept Printable = requires(T obj)
{
	{ obj.ToString() } -> SameAs<String>;
};