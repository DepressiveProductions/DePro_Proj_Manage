#include "Blocks.h"

Blocks::Blocks(void) {}

Blocks::~Blocks(void) {}

void Blocks::init(float floorWidth, float floorHeight, float zPos)
{
	srand(time(0)); // Seed the RNG

	fWidth = floorWidth;
	fHeight = floorHeight;
	fZ = zPos;
}

void Blocks::sendWave()
{
	blocks[0] = new Block();
	blocks[0]->init(randF(fWidth/2,(fWidth/2)+fWidth)  ,  randF(-fHeight/2, fHeight/2)  ,  fZ  ,  randF(0.1f, 2.0f)  ,  randF(0.1f, 2.0f));

	// Spawn-position
		// 20 <= x <= 60
		// -2.5 <= y <= 2.5
		// z = -10
	// 20*5
}

float Blocks::randF(float min, float max)
{
	return ((float)rand()/RAND_MAX)*(max-min) + min; // A random number between randMin and randMax
}

void Blocks::draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient)
{
	blocks[0]->move();
	blocks[0]->draw(sManager, tPipeline, mvStack, vLight, vAmbient);
}