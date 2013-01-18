//Includes:
#include "House.h"

//Counstructor:
Block::Block(void) {}
//Destructor:
Block::~Block(void) {}

void Block::init(array<float,3> pos, float radius)
{
	blockFrame.SetOrigin(pos[0], pos[1], pos[2]+radius);
	position = pos;
}

void Block::draw(MyShaderManager &shaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, M3DVector4f vColor, M3DVector4f vSpecular, GLBatch &batch)
{
	mvStack.PushMatrix();													//Push to get a matrix that we are free to mess with
	mvStack.MultMatrix(blockFrame);											//Transform the top of the model view stack with the new frame position
	shaders.useADSVert(vColor, vAmbient, vSpecular, vLightPos, tPipeline);	//Use the ADS shader that interpolates between vertices
	batch.Draw();															//Draw the batch with the above specified shader
	mvStack.PopMatrix();													//Pop back to the previous matrix, reverting the changes
}

void Block::setPosition(array<float,3> pos, float radius)
{
	blockFrame.SetOrigin(pos[0], pos[1], pos[2]+radius);
	position = pos;
}

array<float,3> Block::getPosition()
{
	return position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// End of class

//Constructor:
Building::Building(void) {}
//Destructor:
Building::~Building(void) {}

void Building::init(vector< array<float,3> > positions, float radius)
{
	//Create all the blocks:
	for (unsigned int i = 0 ; i < positions.size() ; i++)
	{
		blocks.push_back(new Block());
		blocks[blocks.size()-1]->init(positions[i], radius);
	}

	//Walls:
	// Get upper left corner and lower right corner
	unsigned int UL = 0; // Index of upper left corner
	unsigned int LR = 0; // Index of lower right corner
	for (unsigned int i=0; i<positions.size(); i++)
	{
		if (positions[i][0] <= positions[UL][0] && positions[i][1] >= positions[UL][1])
			UL = i;
		if (positions[i][0] >= positions[LR][0] && positions[i][1] <= positions[LR][1])
			LR = i;
	}

	//Define walls
	setWalls(positions[UL][0], positions[UL][1], positions[LR][0], positions[LR][1], radius);

	//Nodes:
	array<float, 3> node = {positions[UL][0]-(2*radius), positions[UL][1]+(2*radius), positions[UL][2]};
	pathNodes[0] = node;

	node[0] = positions[UL][0]-(2*radius);
	node[1] = positions[LR][1]-(2*radius);
	pathNodes[1] = node;
	
	node[0] = positions[LR][0]+(2*radius);
	node[1] = positions[UL][1]+(2*radius);
	pathNodes[2] = node;
	
	node[0] = positions[LR][0]+(2*radius);
	node[1] = positions[LR][1]-(2*radius);
	pathNodes[3] = node;
	
	node[0] = ((positions[LR][0] - positions[UL][0])/2) + positions[UL][0]; // Rigth in the middle of upper left x and lower right x
	node[1] = positions[UL][1] + (2*radius);
	pathNodes[4] = node;
}

void Building::drawBlocks(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, M3DVector4f vColor, M3DVector4f vSpecular, GLBatch &batch)
{
	for (unsigned int i = 0 ; i < blocks.size() ; i++)
		blocks[i]->draw(emilShaders, tPipeline, vLightPos, mvStack, vAmbient, vColor, vSpecular, batch);
}

// Upper left and lower right corner
void Building::setWalls(float x1, float y1, float x2, float y2, float radius)
{
	// Adding 0.2 is padding, prevents characters walking with half their body inside the building
	if (x1 >= x2)
	{	
		walls[0] = x2-(radius+0.2f);
		walls[1] = x1+(radius+0.2f);
	}
	else if (x1 < x2)
	{
		walls[0] = x1-(radius+0.2f);
		walls[1] = x2+(radius+0.2f);
	}
	if (y1 >= y2)
	{
		walls[2] = y2-(radius+0.2f);
		walls[3] = y1+(radius+0.2f);
	}
	else if (y1 < y2)
	{
		walls[2] = y1-(radius+0.2f);
		walls[3] = y2+(radius+0.2f);
	}
}

vector< array<float,3> > Building::getPositions()
{
	vector< array<float,3> > vTempRet;
	for (unsigned int i = 0 ; i < blocks.size() ; i++)
		vTempRet.push_back(blocks[i]->getPosition());
	return vTempRet;
}

array<float, 4> Building::getWalls()
{
	return walls;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// End of class

//Constructor:
House::House(void) {}
//Destructor:
House::~House(void) {}

//Loop through every building of the building type, and draw them:
void House::drawAll(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient)
{
	for (unsigned int i = 0 ; i < buildings.size() ; i++) //Loop through every position in the vector ...
	{
		buildings[i]->drawBlocks(emilShaders, tPipeline, vLightPos, mvStack, vAmbient, vColor, vSpecular, houseBatch); //... and draw the building at those positions
	}
}

void House::removeBuilding(unsigned int index)
{
	delete buildings[index];
	buildings.erase(buildings.begin()+(int)index);
}

//Create a building - not a building type, a factual building:
void House::create(vector< array<float, 3> > pos)
{
	//Create and initiate the new building object:
	buildings.push_back(new Building());
	buildings[buildings.size()-1]->init(pos, radius);
}

array<array<float, 3>, 5> Building::getNodes()
{
	return pathNodes;
}

array<float, 3> Building::getDoorNode() // Get coord for the node outside the door
{
	return pathNodes[4];
}

//Initiate a building with specified shinyness and color:
void House::init(GLfloat fRadius, M3DVector4f shine, M3DVector4f color)
{
	//Set the values and make a cube batch with the new radius:
	radius = fRadius;
	setColor(vSpecular, shine);
	setColor(vColor, color);
	gltMakeCube(houseBatch, radius);
}

//Initiate a building type with default shinyness and color:
void House::init(GLfloat fRadius)
{
	//Set radius to the argument and the rest of the values to default stuff, and make a cube:
	radius = fRadius;
	setColor(0.3f, 0.3f, 0.3f, 1.0f);
	setColor(vSpecular, 0.2f, 0.2f, 0.2f, 1.0f);
	gltMakeCube(houseBatch, radius);
}

//Returns the building type color by setting it to &vOut
void House::getColor(M3DVector4f &vOut)
{
	vOut[0] = vColor[0];
	vOut[1] = vColor[1];
	vOut[2] = vColor[2];
	vOut[3] = vColor[3];
}

vector< Building * > House::getBuildings()
{
	return buildings;
}

//Returns the positions of the cubes that make up the buildings of the building type
vector< array<float,3> > House::getPositions()
{
	vector< array<float,3> >vTempRet;
	for (unsigned int b = 0 ; b < buildings.size() ; b++)
		for (unsigned int p ; p < buildings[b]->getPositions().size(); p++)
			vTempRet.push_back(buildings[b]->getPositions()[p]);
	return vTempRet;
}

//Returns the radius of the cubes:
float House::getRadius()
{
	return radius;
}

//Set building type color to specified GLfloats:
void House::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vColor[0] = r;
	vColor[1] = g;
	vColor[2] = b;
	vColor[3] = a;
}

//Set a specified color to specified GLfloats:
void House::setColor(M3DVector4f &vCustColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vCustColor[0] = r;
	vCustColor[1] = g;
	vCustColor[2] = b;
	vCustColor[3] = a;
}

//Set specified color to specified M3DVector4f:
void House::setColor(M3DVector4f &vOldColor, M3DVector4f vNewColor)
{
	vOldColor[0] = vNewColor[0];
	vOldColor[1] = vNewColor[1];
	vOldColor[2] = vNewColor[2];
	vOldColor[3] = vNewColor[3];
}

