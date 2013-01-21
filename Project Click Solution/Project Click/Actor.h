#pragma once

#include <GLBatch.h>

#include <vector>
#include <array>

using std::array;
using std::vector;

class Actor
{
	public:
		Actor(void);
		~Actor(void);

		M3DVector4f vColor;
		GLBatch aBatch;
};