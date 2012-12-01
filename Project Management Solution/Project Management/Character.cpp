#include "Character.h"
#include <iostream>

Character::Character(void)
{
}

Character::~Character(void)
{
}

void Character::init(float charSize, float mvSpeed, M3DVector3f &spawnPos)
{
	size = charSize;
	cFrame.SetOrigin(spawnPos);
	stepSize = 0.1f;// * mvSpeed;
	moving = false;

	gltMakeCube(cBatch, size);
	vColour[0] = 1.0f;
	vColour[1] = 0.0f;
	vColour[2] = 0.0f;
	vColour[3] = 1.0f;
}

void Character::moveTo(M3DVector3f mvPos)
{
	moving = true;
	M3DVector3f position;

	do
	{
		calculateDirection(mvPos);
		cFrame.SetUpVector(direction);
		cFrame.MoveUp(stepSize);
		std::cout << cFrame.GetOriginX() << ", " << cFrame.GetOriginY() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		cFrame.GetOrigin(position);
	}
	while ((position[0] >= mvPos[0]-stepSize) && (position[0] <= mvPos[0]+stepSize)
			&& (position[1] >= mvPos[2]-stepSize) && (position[1] <= mvPos[1]+stepSize));
	moving = false;
}

bool Character::isMoving()
{
	return moving;
}

void Character::calculateDirection(M3DVector3f mvPos)
{
	M3DVector3f sPos = {cFrame.GetOriginX(), cFrame.GetOriginY(), cFrame.GetOriginZ()};
	M3DVector3f ePos;
	for (int i=0; i<3; i++)
		ePos[i] = mvPos[i];
	ePos[2] += size; // Prevent the cube from moving in Z-axis

	m3dSubtractVectors3(direction, ePos, sPos);
}

void Character::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLightPos)
{
	mvStack.PushMatrix();
	mvStack.MultMatrix(cFrame);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, tPipeline.GetModelViewMatrix(), tPipeline.GetProjectionMatrix(), vLightPos, vColour);
	cBatch.Draw();
	mvStack.PopMatrix();
}