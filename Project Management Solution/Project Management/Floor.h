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

class Floor
{
public:
	Floor();
	void init(GLfloat X, GLfloat Y, GLfloat Z, int Width, int Height, GLfloat Scale);
	void setFloorColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setGridColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setGridLineWidth(GLfloat w);
	void toggleGrid();
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	GLfloat position[3];
	float width;
	float height;
	GLfloat scale;
	GLfloat vFloorColour[4];
	GLBatch fBatch;
	
	GLBatch gBatch;
	GLfloat vGridColour[4];
	GLfloat gLineWidth;
	bool drawGrid;
};

