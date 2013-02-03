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

void Blocks::sendWave(int blockAmount)
{
	Globals::nBlocks += blockAmount;
	for (int i=0; i<blockAmount; i++)
	{
		blocks.push_back(new Block);
		blocks[blocks.size()-1]->init(randF(-(fWidth/2),(fWidth/2))  ,  randF(-fHeight/2, fHeight/2)  ,  fZ  ,  randF(1.0f, 2.0f)  ,  randF(1.0f, 2.0f));
	}
}

bool Blocks::remove(float x, float y, float z) // Returns true if a block was removed
{
	for (unsigned int i=0; i<blocks.size(); i++)
	{
		if (blocks[i]->isWithin(x, y, z))
		{
			blocks.erase(blocks.begin() + i);
			return true;
		}
	}

	return false;
}

float Blocks::randF(float min, float max)
{
	float random = (((float)rand()/RAND_MAX)*(max-min)) + min; // A random number between randMin and randMax
	return random;
}

void Blocks::draw(Shaders *sManager, GLGeometryTransform *tPipeline, GLMatrixStack *mvStack, M3DVector4f vLight, M3DVector4f vAmbient)
{
	for (unsigned int i=0; i<blocks.size(); i++)
	{
		blocks[i]->draw(*sManager, *tPipeline, *mvStack, vLight, vAmbient);
	}
}