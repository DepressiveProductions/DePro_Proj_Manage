#include "Background.h"

//Constructor & destructor:

Background::Background(void) {}
Background::~Background(void) {}

//////////////////////////////////////////////////////////////////////////////////////////

//Methods:

void Background::init(float width, float height)
{
	z = -5.0f;

	//Set color to white:
	setColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Calc positions:
	array<float,3> botLeft = {-width/2, -height/2, z};
	array<float,3> botRight = {width/2, -height/2, z};
	array<float,3> topRight = {width/2, height/2, z};
	array<float,3> topLeft = {-width/2, height/2, z};

	//Set positions and initiate the batch:
	setPositions(botLeft, botRight, topRight, topLeft);
	initBatch();
}

void Background::initBatch()
{
	aBatch.Begin(GL_TRIANGLE_FAN, 4);
	aBatch.Vertex3f(positions[0][0], positions[0][1], positions[0][2]);
	aBatch.Vertex3f(positions[1][0], positions[1][1], positions[1][2]);
	aBatch.Vertex3f(positions[2][0], positions[2][1], positions[2][2]);
	aBatch.Vertex3f(positions[3][0], positions[3][1], positions[3][2]);
	aBatch.End();
}

void Background::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline)
{
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vColor);
	aBatch.Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////

//Setters:

void Background::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vColor[0] = r;
	vColor[1] = g;
	vColor[2] = b;
	vColor[3] = a;
}

void Background::setColor(M3DVector4f vNewColor)
{
	vColor[0] = vNewColor[0];
	vColor[1] = vNewColor[1];
	vColor[2] = vNewColor[2];
	vColor[3] = vNewColor[3];
}

void Background::setColor(array<GLfloat,4> vNewColor)
{
	vColor[0] = vNewColor[0];
	vColor[1] = vNewColor[1];
	vColor[2] = vNewColor[2];
	vColor[3] = vNewColor[3];
}

void Background::setPositions(vector< array<float,3> > pos)
{
	positions = pos;
}

void Background::setPositions(array<float,3> pos1, array<float,3> pos2, array<float,3> pos3, array<float,3> pos4)
{
	positions.push_back(pos1);
	positions.push_back(pos2);
	positions.push_back(pos3);
	positions.push_back(pos4);
}

//////////////////////////////////////////////////////////////////////////////////////////

//Getters:


//////////////////////////////////////////////////////////////////////////////////////////