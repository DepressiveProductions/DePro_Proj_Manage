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
	void activateSquare(M3DVector3f &pos);
	void boxActivation(M3DVector3f &startPos, M3DVector3f &endPos);
	void deactivateAllSquares();
	void getSquarePositions(vector< vector<float> > &positions);
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	struct highlighted {GLBatch batch; M3DVector3f pos;};
	vector<highlighted *> hl;
	GLfloat gridScale;
	GLfloat vGridColour[4];
	GLfloat lineWidth;
	GLfloat center[3]; // Coords for the center of floor and center of the full grid
	
	GLBatch markingLines;

	void calculateExactPos(M3DVector3f &pos); // Changes pos to the corresponding square-position
};