#include "Background.h"

//Constructor & destructor:

Background::Background(void) {}
Background::~Background(void) {}

//////////////////////////////////////////////////////////////////////////////////////////

//Methods:

void Background::init(float width, float height)
{
	//Set color to white:
	setColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Initiate the batch:
	initBatch();
}

void Background::initBatch()
{
	z = -5.0f;

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

//////////////////////////////////////////////////////////////////////////////////////////

//Getters:


//////////////////////////////////////////////////////////////////////////////////////////