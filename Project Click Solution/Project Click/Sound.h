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

	void init(const char *szFileName);
	int play(bool loop, ALfloat sourcePos[], ALfloat sourceVel[]);
	void play(bool loop, ALfloat sourcePos[]);
	void play(bool loop);
	void play();
	int endWithError(char *msg, int error = 0);
	void clearSound(); // Goes in main before terminating program
	void clearSoundPartial();

private:
	FILE *fp;
	const char *fileName;
	unsigned char* buf;
	ALuint source;
	ALuint buffer;
	ALCcontext *context;
	ALCdevice *device;
};

