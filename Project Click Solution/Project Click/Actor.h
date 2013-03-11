#pragma once

#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrame.h>

#include <array>

using std::array;

class Actor
{
public:
	Actor(void);
	~Actor(void);

	GLfloat vColor[4];
	GLfloat vShininess[4];
	GLBatch aBatch;
	GLFrame aFrame;
};