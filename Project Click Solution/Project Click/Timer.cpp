#include "Timer.h"

Timer::Timer(void)
{
	startingTime = time(NULL);
}

Timer::~Timer(void)
{
}

float Timer::getElapsedTime()
{
	now = time(NULL);
	return float(difftime(now, startingTime));
}

void Timer::resetTime()
{
	startingTime = time(NULL);
}