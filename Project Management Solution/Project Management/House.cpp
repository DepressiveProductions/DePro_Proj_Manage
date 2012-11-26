//Includes:
#include "House.h"

//Constructor:
House::House(void)
{
}

//Destructor:
House::~House(void)
{
}

//Changing the position to draw a cube at:
void House::nextPos()
{
	int xtype = 1;
	int ytype = 1;
	if (vEndingPos[0] - vStartingPos[0] < 0)
		xtype = -1;
	if (vEndingPos[1] - vStartingPos[1] > 0)
		ytype = -1;
	
	if (xtype*vCurrentPos[0] < xtype*vEndingPos[0])
	{
		vCurrentPos[0] += xtype*radius*2;
	}
	
	else if (ytype*vCurrentPos[1] >= ytype*vEndingPos[1])
	{
		vCurrentPos[0] = vStartingPos[0];
		vCurrentPos[1] -= ytype*radius*2;
	}
}

//Drawing the house:
void House::draw(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, GLBatch &batch)
{
	for(unsigned int i = 0 ; i < cubes ; i++)
	{
		mvStack.PushMatrix();
		
		houseFrame.SetOrigin(positions[i]);
		mvStack.MultMatrix(houseFrame);
		emilShaders.useADSVert(vColor, vAmbient, vSpecular, vLightPos, tPipeline);
		batch.Draw();
		
		mvStack.PopMatrix();
	}
}

void House::drawAll(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient)
{
	for (unsigned int i = 0 ; i < houses.size() ; i++)
	{
		houses[i]->draw(emilShaders, tPipeline, vLightPos, mvStack, vAmbient, houseBatch);
	}
}

//Creating the house:
void House::create(vector< M3DVector3f > pos)
{
	houses.push_back(new House());
	houses[houses.size()-1]->cubes = pos.size();
	for (unsigned int i = 0 ; i < pos.size() ; i++)
	{
		houses[houses.size()-1]->positions.push_back(pos[i]);
	}
	houses[houses.size()-1]->inherit(this->radius, this->vSpecular, this->vColor);
}

void House::inherit(GLfloat fRadius, M3DVector4f shine, M3DVector4f color)
{
	radius = fRadius;
	
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		vSpecular[i] = shine[i];
		vColor[i] = color[i];
	}
}


//Constructor, kind of:
void House::init(GLfloat fRadius, M3DVector4f shine, M3DVector4f color)
{
	radius = fRadius;
	setColor(vSpecular, shine);
	setColor(vColor, color);
	gltMakeCube(houseBatch, radius);
}

void House::init(GLfloat fRadius)
{
	radius = fRadius;
	setColor(0.3f, 0.3f, 0.3f, 1.0f);
	setColor(vSpecular, 0.2f, 0.2f, 0.2f, 1.0f);
	gltMakeCube(houseBatch, radius);
}

//Getters:
void House::getColor(M3DVector4f &vOut)
{
	vOut[0] = vColor[0];
	vOut[1] = vColor[1];
	vOut[2] = vColor[2];
	vOut[3] = vColor[3];
}

void House::getPositions(M3DVector4f &vOutStarting, M3DVector4f &vOutEnding)
{
	for(int i = 0 ; i < 3 ; i++)
	{
		vOutStarting[i] = vStartingPos[i];
		vOutEnding[i] = vEndingPos[i];
	}
}

float House::getRadius()
{
	return radius;
}

//Setters:
void House::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vColor[0] = r;
	vColor[1] = g;
	vColor[2] = b;
	vColor[3] = a;
}

void House::setColor(M3DVector4f &vCustColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vCustColor[0] = r;
	vCustColor[1] = g;
	vCustColor[2] = b;
	vCustColor[3] = a;
}

void House::setColor(M3DVector4f &vOldColor, M3DVector4f vNewColor)
{
	vOldColor[0] = vNewColor[0];
	vOldColor[1] = vNewColor[1];
	vOldColor[2] = vNewColor[2];
	vOldColor[3] = vNewColor[3];
}

