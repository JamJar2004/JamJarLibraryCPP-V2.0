#pragma once

template<typename T, typename... Args>
using Function = T(*)(Args...);

template<typename C, typename T, typename... Args>
using Method = T(C::*)(Args...);

//class DynamicFunction
//{
//private:
//	template<typename T, typename... Args>
//	class Caller
//	{
//	private:
//		Function<T, Args...> m_function;
//	public:
//		Caller(Function<T, Args...> function) : m_function(function) {}
//
//		void Call(Args&&... args);
//	};
//public:
//	DynamicFunction() {}
//};