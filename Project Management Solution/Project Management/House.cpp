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
}

//Drawing the house:
void House::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack)
{
	vCurrentPos[0] = vStartingPos[0];
	vCurrentPos[1] = vStartingPos[1];
	vCurrentPos[2] = vStartingPos[2];
	
	for(int i = 0 ; i < cubes ; i++)
	{
		mvStack.PushMatrix();
		
		mvStack.MultMatrix(houseFrame);
		shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, tPipeline.GetModelViewMatrix(), tPipeline.GetProjectionMatrix(), vLightPos, vColor);
		houseBatch.Draw();
		
		nextPos();
		houseFrame.SetOrigin(vCurrentPos);
		
		mvStack.PopMatrix();
	}
}

//Constructor, kind of:
void House::init(M3DVector4f vStartingPos, M3DVector4f vEndingPos, GLfloat fRadius)
{
	setColor(0.0f, 1.0f, 0.0f, 1.0f);
	radius = fRadius;
	for(int i = 0 ; i < 3 ; i++)
	{
		this->vStartingPos[i] = vStartingPos[i];
		this->vEndingPos[i] = vEndingPos[i];
	}
	gltMakeCube(houseBatch, radius);
	cubes = 1;
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

