#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilisecondsElapsed();
	double GetMicrosecondsElapsed();
	double GetTimeUntilEnd(float maxTime);
	double getSecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
private:
	bool isrunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
#else
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> stop;
#endif
};