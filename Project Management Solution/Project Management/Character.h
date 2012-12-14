#pragma once
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <GLFrame.h>
#include <GLBatch.h>
#include <vector>
#include <array>
#include <thread>
using std::vector;
using std::array;
using std::thread;

class Character
{
public:
	Character(void);
	~Character(void);
	void init(float charSize, float mvSpeed, M3DVector3f &spawnPos);
	void move();
	void moveTo(M3DVector3f mvPos);
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLightPos);

private:
	GLFrame cFrame;
	GLBatch cBatch;
	M3DVector3f vColour;
	M3DVector3f direction;
	M3DVector3f goalPos;
	float size;

	void calculateDirection(M3DVector3f mvPos);
	float stepSize;
};

