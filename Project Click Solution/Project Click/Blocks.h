#pragma once
#include "Block.h"
#include <random>
#include <ctime>
#include <iostream>
#include <array>
using std::array;

class Blocks
{
public:
	Blocks(void);
	~Blocks(void);
	void init(float floorWidth, float floorHeight, float zPos);
	void sendWave();
	float randF(float min, float max);
	void draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient);

	float randMin, randMax;
	array<Block *, 10> blocks;
	float fWidth, fHeight, fZ;
};

