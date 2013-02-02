#include "Blocks.h"

Blocks::Blocks(void) {}

Blocks::~Blocks(void) {}

void Blocks::init()
{
	typedef std::mt19937 myRNG;
	
}

void Blocks::sendWave(int blockAmount)
{
	srand(time(NULL));
}

void Blocks::draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient)
{
	
}
