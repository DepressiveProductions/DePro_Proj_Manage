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
	M3DVector4f vShininess;
	GLBatch aBatch;
	GLFrame aFrame;
};