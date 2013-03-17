#pragma once
#include <ctime>

class Timer
{
public:
	Timer(void);
	~Timer(void);
	double getElapsedTime();
	void resetTime();

private:
	time_t now;
	time_t startingTime;
};

