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
	moveTo(spawnPos);
	stepSize = 0.02f;// * mvSpeed;

	gltMakeCube(cBatch, size);
	vColour[0] = 1.0f;
	vColour[1] = 0.0f;
	vColour[2] = 0.0f;
	vColour[3] = 1.0f;
}

void Character::move()
{
	M3DVector3f pos;
	cFrame.GetOrigin(pos);
	if (!(pos[0] < goalPos[0] + (stepSize/1.5f) && pos[0] > goalPos[0]-(stepSize/1.5f)) && !(pos[1] < goalPos[1] + (stepSize/1.5f) && pos[1] > goalPos[1]-(stepSize/1.5f)))
		cFrame.MoveUp(stepSize);
}

void Character::moveTo(M3DVector3f mvPos)
{
	for (int i=0; i<3; i++)
		goalPos[i] = mvPos[i];
	calculateDirection(goalPos);
	cFrame.SetUpVector(direction);
}

void Character::calculateDirection(M3DVector3f mvPos)
{
	M3DVector3f sPos = {cFrame.GetOriginX(), cFrame.GetOriginY(), cFrame.GetOriginZ()};
	M3DVector3f ePos;
	for (int i=0; i<3; i++)
		ePos[i] = mvPos[i];

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