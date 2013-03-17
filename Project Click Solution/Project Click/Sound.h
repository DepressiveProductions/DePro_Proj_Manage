#pragma once
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <AL/al.h>
#include <AL/alc.h>

class Sound
{
public:
	Sound(void);
	~Sound(void);

	int init(const char *szFileName);
	int play(ALfloat sourcePos[], ALfloat sourceVel[]);
	void play(ALfloat sourcePos[]);
	void play();
	int endWithError(char *msg, int error = 0);
	void clearSound(); // Goes in main before terminating program

private:
	FILE *fp;
	unsigned char* buf;
	ALuint source;
	ALuint buffer;
	ALCcontext *context;
	ALCdevice *device;
};

