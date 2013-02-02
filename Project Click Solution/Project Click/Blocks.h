#pragma once
#include "Block.h"
#include <array>
#include <random>
#include <ctime>
using std::array;

class Blocks
{
public:
	Blocks(void);
	~Blocks(void);
	void init();
	void sendWave(int blockAmount);
	void draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient);
	

};

