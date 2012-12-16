#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <vector>
using std::vector;

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
	void init(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat Scale);
	void setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setLineWidth(GLfloat w);

	void boxActivation(M3DVector3f &startPos, M3DVector3f &endPos);
	void deactivateAllSquares();
	void getSquarePositions(vector< vector<float> > &positions);
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	GLBatch hlBatch;
	vector< vector<float> > squarePositions;
	//vector<highlighted *> hl; // Highlighted squares
	GLfloat gridScale; // Size of the squares, radius
	GLfloat vGridColour[4];
	GLfloat lineWidth; // For grid
	GLfloat center[3]; // Coords for the center of floor and full grid
	
	GLBatch markingLines; // Batch for marking-box

	void activateSquares();
	void setUpperLeft(M3DVector3f &sPos, M3DVector3f &ePos);
	void calculateExactPos(M3DVector3f &pos); // Changes pos to the corresponding square-position
};