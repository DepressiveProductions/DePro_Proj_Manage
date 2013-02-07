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

void Blocks::spawnBlocks(int blockAmount, float mvSpeed)
{
	Globals::nBlocks += blockAmount;
	for (int i=0; i<blockAmount; i++)
	{
		float x, y, w, h;
		
		x = randF(-(fWidth/2)+1.0f,(fWidth/2)-1.0f);
		y = randF(-(fHeight/2)+1.0f,(fHeight/2)-1.0f);
		w = randF(0.25f, 1.0f);
		h = randF(0.25f, 1.0f);
		bool valid = true;
		
		unsigned int size = blocks.size();
		for (unsigned int j=0; j<size; j++)
		{
			if (blocks[j]->isWithin(x-w, y+h, fZ) || //topleft
				blocks[j]->isWithin(x+w, y+h, fZ) || //topright
				blocks[j]->isWithin(x-w, y-h, fZ) || //botleft
				blocks[j]->isWithin(x+w, y-h, fZ) || //botright
				blocks[j]->isWithin(x, y, fZ)) // mid
			{
				i--;
				valid = false;
				break;
			}
		}

		if (mvSpeed > 0.0f)
		{
			x += fWidth; // Place it further right, will make it come in from the right side
		}

		if (valid)
		{
			blocks.push_back(new Block);
			blocks[blocks.size()-1]->init(x, y, fZ, 2*w, 2*h, mvSpeed);
			blocks[blocks.size()-1]->setColor(randF(0.0f,1.0f), randF(0.0f,1.0f), randF(0.0f,1.0f), 1.0f);
		}
		
		if (blocks.size() == 0)
		{
			blocks.push_back(new Block);
			blocks[blocks.size()-1]->init(x, y, fZ, 2*w, 2*h, mvSpeed);
			blocks[blocks.size()-1]->setColor(randF(0.0f,1.0f), randF(0.0f,1.0f), randF(0.0f,1.0f), 1.0f);
		}
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

void Blocks::removeAll()
{
	blocks.clear();
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
		blocks[i]->move();
		blocks[i]->draw(*sManager, *tPipeline, *mvStack, vLight, vAmbient);
	}
}