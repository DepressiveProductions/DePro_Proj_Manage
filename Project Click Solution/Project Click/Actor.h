#pragma once

#include <GLBatch.h>
#include <GLFrame.h>

#include <array>

using std::array;

class Actor
{
public:
	Actor(void);
	~Actor(void);

	M3DVector4f vColor;
	GLBatch aBatch;
	GLFrame aFrame;
};