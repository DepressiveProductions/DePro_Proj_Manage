#pragma once
#include "Block.h"
#include "Globals.h"
#include <random>
#include <iostream>
#include <array>
#include <vector>
#include <math.h>

using std::array;
using std::vector;

class Blocks
{
public:
	Blocks(void);
	~Blocks(void);

	void init(float floorWidth, float floorHeight, float zPos);
	void sendWave(int blockAmount, float moveSpeed);
	bool remove(float x, float y, float z);
	void removeAll();
	float randF(float min, float max);
	void draw(Shaders *sManager, GLGeometryTransform *tPipeline, GLMatrixStack *mvStack, M3DVector4f vLight, M3DVector4f vAmbient);

	array<array<GLfloat,4>,5> colorPalette;

	vector<Block *> blocks;
	float fWidth, fHeight, fZ;
};

