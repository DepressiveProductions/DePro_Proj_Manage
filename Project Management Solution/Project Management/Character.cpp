#include "Character.h"
#include <iostream>

Character::Character(void)
{
}

Character::~Character(void)
{
}

void Character::init(float charSize, float mvSpeed, array<float, 3> spawnPos)
{
	size = charSize;
	cFrame.SetOrigin(spawnPos[0], spawnPos[1], spawnPos[2]);
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
	//if (!(pos[0] < goalPos[0] + (stepSize/1.5f) && pos[0] > goalPos[0]-(stepSize/1.5f)) && !(pos[1] < goalPos[1] + (stepSize/1.5f) && pos[1] > goalPos[1]-(stepSize/1.5f)))
	if (!(abs(pos[0]-goalPos[0]) <= stepSize) && !(abs(pos[1]-goalPos[1]) <= stepSize))
		cFrame.MoveUp(stepSize);
	//else if(((pos[0]-goalPos[0]) == 0) && ((pos[1]-goalPos[1]) == 0))
		
	else if((abs(pos[0]-goalPos[0]) <= stepSize) && (abs(pos[1]-goalPos[1]) <= stepSize))
		cFrame.SetOrigin(goalPos[0], goalPos[1], goalPos[2]);
}

void Character::moveTo(array<float, 3> mvPos)
{
	for (int i=0; i<3; i++)
		goalPos[i] = mvPos[i];
	calculateDirection(goalPos);
	cFrame.SetUpVector(direction[0], direction[1], direction[2]);
}

void Character::calculateDirection(array<float, 3> mvPos)
{
	array<float, 3> sPos = {cFrame.GetOriginX(), cFrame.GetOriginY(), cFrame.GetOriginZ()};
	array<float, 3> ePos;
	for (int i=0; i<3; i++)
		ePos[i] = mvPos[i];

	for (int i=0; i<3; i++)
		direction[i] = ePos[i] - sPos[i];
	float vLength = sqrt(pow(direction[0], 2) + pow(direction[1], 2));
	direction[0] /= vLength;
	direction[1] /= vLength;
}

void Character::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLightPos)
{
	mvStack.PushMatrix();
	mvStack.MultMatrix(cFrame);
	shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, tPipeline.GetModelViewMatrix(), tPipeline.GetProjectionMatrix(), vLightPos, vColour);
	cBatch.Draw();
	mvStack.PopMatrix();
}