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
	void init(float charSize, float mvSpeed, array<float, 3> spawnPos);
	void move();
	void moveTo(array<float, 3> mvPos);
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLightPos);

private:
	GLFrame cFrame;
	GLBatch cBatch;
	M3DVector4f vColour;
	array<float, 3> direction;
	array<float, 3> goalPos;
	float size;

	void calculateDirection(array<float, 3> mvPos);
	float stepSize;
};

