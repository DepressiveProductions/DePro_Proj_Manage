#pragma once
#include "Block.h"
#include <random>
#include <ctime>
#include <iostream>
#include <array>
#include <vector>
using std::array;
using std::vector;

class Blocks
{
public:
	Blocks(void);
	~Blocks(void);

	void init(float floorWidth, float floorHeight, float zPos);
	void sendWave(int blockAmount);
	bool remove(float x, float y, float z);
	float randF(float min, float max);
	void draw(Shaders *sManager, GLGeometryTransform *tPipeline, GLMatrixStack *mvStack, M3DVector4f vLight, M3DVector4f vAmbient);

	vector<Block *> blocks;
	float fWidth, fHeight, fZ;
};

