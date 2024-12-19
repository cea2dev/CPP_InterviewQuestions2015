#include "Timer.h"
#include <stdio.h>

//dtor
Timer::Timer()
	:m_Init64(0), m_dTime(0.0), m_bRunning(false)
{
	//some more WIN32 stuff
	//the performance counter is created once on system boot
	//and it constant, so we can query once in the constructor
	//and store it away
	if(!QueryPerformanceFrequency(&m_largeInt))
		printf("QueryPerformanceFrequency failed\n");

	//this member is what's going to be handling how we want to
	//see our time (as milliseconds, hence the / 1000.0)
	m_dFreq = double(m_largeInt.QuadPart) / 1000.0;
}

Timer::~Timer()
{
}

void Timer::Start()
{
	if(m_bRunning)
		return;
	this->SetInitial();
	m_bRunning = true;
}

void Timer::Stop()
{
	if(!m_bRunning)
		return;
	m_dTime += GetCount();
	m_bRunning = false;
}

void Timer::Reset()
{
	m_Init64   = 0;
	m_dTime    = 0.0;
	m_bRunning = false;
}

bool Timer::IsRunning()
{
	return m_bRunning;
}

double Timer::GetTime()
{
	if(m_bRunning)
	{
		m_dTime += this->GetCount();
		this->SetInitial();
	}

	return m_dTime;
}

double Timer::GetSeconds()
{
	return this->GetTime() / 1000.0;
}

void Timer::SetInitial()
{
	//initial value of the perforamce counter when we start
	//the timer, store it away
	//elapsed time will basically be current time - this
	QueryPerformanceCounter(&m_largeInt);
	m_Init64 = m_largeInt.QuadPart;
}

double Timer::GetCount()
{
	//elapsed time - time when timer started
	//returned as double in milliseconds
	QueryPerformanceCounter(&m_largeInt);
	return double (m_largeInt.QuadPart - m_Init64) / m_dFreq;
}
