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

		vector< array<float,3> > getPositions();

		vector<array<float,3>> positions;
		M3DVector4f vColor;
		GLBatch aBatch;

};

