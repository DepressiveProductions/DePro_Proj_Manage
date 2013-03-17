#include "Timer.h"

Timer::Timer(void)
{
	startingTime = time(NULL);
}

Timer::~Timer(void)
{
}

double Timer::getElapsedTime()
{
	now = time(NULL);
	return difftime(now, startingTime);
}

void Timer::resetTime()
{
	startingTime = time(NULL);
}