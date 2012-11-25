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

class Grid
{
public:
	Grid();
	void init(GLfloat Scale, GLfloat X, GLfloat Y);
	void init(GLfloat Scale, GLfloat X, GLfloat Y, int SquareAmountX, int SquareAmountY);
	void setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setLineWidth(GLfloat width);
	void generate();

	GLBatch gBatch;
	GLfloat vGridColour[4];
	GLfloat position[3];

private:
	GLfloat gridScale;
	GLfloat lineWidth;
	int squaresX, squaresY;
	bool isSquare;
};