#pragma once

#include "../String.hpp"

//using Constructor = Function<T>;

class Reflect
{
public:
	template<typename T>
	class Field
	{
	private:
		T m_value;
	public:
		template<typename... Args>
		Field(Args&&... args) requires ConstructibleFrom<Args...> : m_value(args...) {}
	};
};