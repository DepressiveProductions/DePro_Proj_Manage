#include "Blocks.h"

Blocks::Blocks(void) {}

Blocks::~Blocks(void) {}

void Blocks::init(float floorWidth, float floorHeight, float zPos)
{
	srand(time(0)); // Seed the RNG

	fWidth = floorWidth;
	fHeight = floorHeight;
	fZ = zPos;

	//Palette at: http://www.colourlovers.com/palette/694737/Thought_Provoking

	//Yellowish ("thought by some")
	colorPalette[0][0] = 236.0f/255.0f;
	colorPalette[0][1] = 208.0f/255.0f;
	colorPalette[0][2] = 120.0f/255.0f;
	colorPalette[0][3] = 1.0f;
	
	//Orangeish ("thoughtful brick")
	colorPalette[1][0] = 217.0f/255.0f;
	colorPalette[1][1] = 91.0f/255.0f;
	colorPalette[1][2] = 67.0f/255.0f;
	colorPalette[1][3] = 1.0f;

	//Redish ("Thoughtless")
	colorPalette[2][0] = 192.0f/255.0f;
	colorPalette[2][1] = 41.0f/255.0f;
	colorPalette[2][2] = 66.0f/255.0f;
	colorPalette[2][3] = 1.0f;

	//Purpleish ("thought you were")
	colorPalette[3][0] = 84.0f/255.0f;
	colorPalette[3][1] = 36.0f/255.0f;
	colorPalette[3][2] = 55.0f/255.0f;
	colorPalette[3][3] = 1.0f;

	//Greenish ("Thoughtless")
	colorPalette[4][0] = 83.0f/255.0f;
	colorPalette[4][1] = 119.0f/255.0f;
	colorPalette[4][2] = 122.0f/255.0f;
	colorPalette[4][3] = 1.0f;
}

void Blocks::sendWave(int blockAmount, float moveSpeed)
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

		if (blocks.size() == 0) {
			valid = true;
		}

		if (moveSpeed > 0.0f) {
			x += fWidth;
		}

		if (valid) {
			blocks.push_back(new Block);
			blocks[blocks.size()-1]->blocktype = int(randF(0.0f,4.0f));
			blocks[blocks.size()-1]->init(x, y, fZ, 2*w, 2*h, moveSpeed);
			blocks[blocks.size()-1]->setColor(colorPalette[blocks[blocks.size()-1]->blocktype][0], 
											  colorPalette[blocks[blocks.size()-1]->blocktype][1], 
											  colorPalette[blocks[blocks.size()-1]->blocktype][2], 
											  1.0f);
		}
	}
}

bool Blocks::remove(float x, float y, float z) // Returns true if a block was removed
{
	for (unsigned int i=0; i<blocks.size(); i++)
	{
		if (blocks[i]->isWithin(x, y, z))
		{
			Globals::score += pow(2, blocks[i]->blocktype);
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
		
		if (blocks[i]->hasPassed(fWidth)) {
			Globals::lives --;
			Globals::nBlocks--;
			blocks.erase(blocks.begin()+i);
		}
	}
}