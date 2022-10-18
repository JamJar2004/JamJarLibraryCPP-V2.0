#pragma once

#include "Clock.hpp"
#include "../Delegate.hpp"

class StopWatch
{
private:
	DateTime m_startTime;
	DateTime m_stopTime;

	Boolean m_started;
public:
	StopWatch() : m_startTime(), m_stopTime(), m_started(false) {}

	void Start() 
	{
		if(m_started)
			return;

		m_startTime = Clock::GetTime(); 
		m_started = true;
	}
	
	Duration Stop()
	{
		if(!m_started)
			return Duration::Zero;

		m_stopTime = Clock::GetTime();
		return m_stopTime - m_startTime;
	}
};

class ScopeStopWatch
{
private:
	Event<Duration> m_destroyedEvent;

	StopWatch m_stopWatch;
public:
	template<SameAs<Function<void, Duration>>... Args>
	ScopeStopWatch(Args... handlers) 
	{
		m_destroyedEvent.AddHandler(handlers...);
		m_stopWatch.Start(); 
	}

	~ScopeStopWatch() 
	{
		Duration duration = m_stopWatch.Stop();
		m_destroyedEvent(duration);
	}
};