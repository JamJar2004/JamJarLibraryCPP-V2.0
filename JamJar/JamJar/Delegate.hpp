#pragma once

//#include "Data/Collections/Sets/HashSet.hpp"
//
template<typename T, typename... Args>
using Function = T(*)(Args...);

template<typename C, typename T, typename... Args>
using Method = T(C::*)(Args...);
//
//template<typename... Args>
//class Event
//{
//private:
//	HashSet<Function<void, Args...>> m_handlers;
//public:
//	Event() {}
//
//	void AddHandler(Function<void, Args...> handler) { m_handlers.Add(handler); }
//
//	void RemoveHandler(Function<void, Args...> handler) { m_handlers.Remove(handler); }
//
//	void operator()(Args... args) const
//	{
//		for(Function<void, Args...> handler : m_handlers)
//			handler(args...);
//	}
//};

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