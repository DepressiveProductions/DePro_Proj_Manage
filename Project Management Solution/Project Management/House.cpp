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
	
	if(vCurrentPos[0] < vEndingPos[0])
	{
		vCurrentPos[0] += radius*2;
	}

	else if(vCurrentPos[1] >= vEndingPos[1])
	{
		vCurrentPos[0] = vStartingPos[0];
		vCurrentPos[1] -= radius*2;
	}
}

//Drawing the house:
void House::draw(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient)
{
	vCurrentPos[0] = vStartingPos[0];
	vCurrentPos[1] = vStartingPos[1];
	vCurrentPos[2] = vStartingPos[2]+radius;
	houseFrame.SetOrigin(vCurrentPos);

	for(int i = 0 ; i < cubes ; i++)
	{
		mvStack.PushMatrix();
		
		mvStack.MultMatrix(houseFrame);
		emilShaders.useADSVert(vColor, vAmbient, vSpecular, vLightPos, tPipeline);
		houseBatch.Draw();
		
		nextPos();
		houseFrame.SetOrigin(vCurrentPos);
		
		mvStack.PopMatrix();
	}
}

//Creating the house:
void House::create(M3DVector4f vStartingPos, M3DVector4f vEndingPos, int iCubes)
{
	setColor(vSpecular, 0.1f, 0.1f, 0.1f, 1.0f);
	setColor(0.3f, 0.3f, 0.3f, 1.0f);

	cubes = iCubes;
	for(int i = 0 ; i < 3 ; i++)
	{
		this->vStartingPos[i] = vStartingPos[i];
		this->vEndingPos[i] = vEndingPos[i];
	}
}


//Constructor, kind of:
void House::init(GLfloat fRadius)
{
	radius = fRadius;
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

void House::setColor(M3DVector4f vCustColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vCustColor[0] = r;
	vCustColor[1] = g;
	vCustColor[2] = b;
	vCustColor[3] = a;
}

