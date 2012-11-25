#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include "Grid.h"
#include <vector>

using std::vector;

class Floor
{
public:
	Floor();
	void init(int Width, int Height, GLfloat Scale);
	void setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void generate();
	void addHLSquare(GLfloat x, GLfloat y);
	
	GLBatch fBatch;
	Grid grid;
	vector<Grid *> hlGrid;
	GLfloat vFloorColour[4];

private:
	float width;
	float height;
	float scale;
};

