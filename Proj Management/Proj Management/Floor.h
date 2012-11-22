#pragma once
#include "OGLIncludes.h"

class Floor
{
public:
	Floor(GLfloat Width, GLfloat Height, GLfloat Scale);
	void setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	GLBatch Batch;
	GLfloat vFloorColour[4];

private:
	float width;
	float height;
	float scale;

	void generate();
};

