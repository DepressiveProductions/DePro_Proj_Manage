#include "Floor.h"
#include "OGLIncludes.h"

// Publics
GLBatch Batch;

// Privates
GLfloat width;
GLfloat height;
GLfloat scale;
GLfloat vFloorColour[4];

Floor::Floor(GLfloat Width, GLfloat Height, GLfloat Scale)
{
	width = Width;
	height = Height;
	scale = Scale;

	generate();
}

void Floor::setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vFloorColour[0] = r;
	vFloorColour[1] = g;
	vFloorColour[2] = b;
	vFloorColour[3] = a;
}

void Floor::generate()
{

	GLfloat vVerts[] = {-(width/2), -(height/2), 0.0f,
						-(width/2), (height/2), 0.0f,
						(width/2), -(height/2), 0.0f,
						(width/2), (height/2), 0.0f};
	Batch.Begin(GL_TRIANGLE_STRIP, 4);
	Batch.CopyVertexData3f(vVerts);
	Batch.End();
}