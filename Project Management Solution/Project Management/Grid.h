#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <vector>
#include <array>
using std::vector;
using std::array;

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
	void init(float centerX, float centerY, float centerZ, float Scale);
	void setColour(float r, float g, float b, float a);
	void setLineWidth(float w);

	void boxActivation(array<float, 3> &startPos, array<float, 3> &endPos);
	void deactivateAllSquares();
	void getSquarePositions(vector<array<float,3>> &positions);
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	GLBatch hlBatch;
	vector< array<float, 3> > squarePositions;
	float gridScale; // Size of the squares, radius
	float vGridColour[4];
	float lineWidth; // For grid
	float center[3]; // Coords for the center of floor and full grid
	
	GLBatch markingLines; // Batch for marking-box

	void activateSquares();
	void setUpperLeft(array<float, 3> &sPos, array<float, 3> &ePos);
	void calculateExactPos(array<float, 3> &pos); // Changes pos to the corresponding square-position
};