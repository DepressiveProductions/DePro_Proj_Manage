#pragma once
#include <cstdlib>
#include <map>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <AL/al.h>
#include <AL/alc.h>

class Sound
{
public:
	Sound(void);
	~Sound(void);

	int init();
	int load(const char *szFileName);

	int play(const char *key, bool loop, ALfloat sourcePos[], ALfloat sourceVel[]);
	void play(const char *key, bool loop, ALfloat sourcePos[]);
	void play(const char *key, bool loop);
	void play(const char *key);
	
	void stop(const char *key);
	int endWithError(char *msg, int error = 0);
	void clearSound(); // Goes in main before terminating program
	//void clearSoundPartial();

private:
	std::vector<const char *> fileNames;
	std::map<const char *, FILE *> fp;
	std::map<const char *, unsigned char *> buf;
	std::map<const char *, ALuint> source;
	std::map<const char *, ALuint> buffer;
	ALCcontext *context;
	ALCdevice *device;
};

