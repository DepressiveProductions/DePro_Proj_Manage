#include "Background.h"

//Constructor & destructor:

Background::Background(void) {}
Background::~Background(void) {}

//////////////////////////////////////////////////////////////////////////////////////////

//Methods:

void Background::init(float width, float height, float z)
{
	//Set color to white:
	setColor(1.0f, 1.0f, 1.0f, 1.0f);

	this->width = width;
	this->height = height;
	this->z = z;

	//Initiate the batch:
	initBatch();
}

void Background::initBatch()
{
	z = -10.0f;

	GLfloat vVerts[] = {-width/2, -height/2, z, //Botleft
						width/2, -height/2, z,	//Botright
						width/2, height/2, z,	//Topright
						-width/2, height/2, z};	//Topleft

	aBatch.Begin(GL_TRIANGLE_FAN, 4);
	aBatch.CopyVertexData3f(vVerts);
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

//////////////////////////////////////////////////////////////////////////////////////////

//Getters:


//////////////////////////////////////////////////////////////////////////////////////////