#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//my simple Timer class
//uses WIN32 API

//exposes time through GetTime() (milliseconds) and GetSeconds() (GetTime() / 1000.f)
class Timer
{
private:
	Timer(const Timer&);
	Timer& operator=(const Timer&);

public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Reset();

	double GetTime();
	double GetSeconds();

	bool IsRunning();

private:
	void SetInitial();
	double GetCount();

	//basically a struct for a representing a large integer
	//using some tricks with unions/internal structs to represent itself
	LARGE_INTEGER m_largeInt;

	__int64       m_Init64;
	double		  m_dFreq;
	double        m_dTime;
	bool          m_bRunning;
};