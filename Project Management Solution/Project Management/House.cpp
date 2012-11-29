//Includes:
#include "House.h"
#include <iostream>

//Constructor:
House::House(void)
{
}

//Destructor:
House::~House(void)
{
}

//Draw building:
void House::draw(MyShaderManager &shaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, GLBatch &batch)
{
	for(unsigned int i = 0 ; i < cubes ; i++)
	{
		mvStack.PushMatrix(); //Push to get a matrix that we are free to mess with
		
		houseFrame.SetOrigin(positions[i][0], positions[i][1], positions[i][2]+radius); //Change the frame position
		mvStack.MultMatrix(houseFrame); //Transform the top of the model view stack with the new frame position
		shaders.useADSVert(vColor, vAmbient, vSpecular, vLightPos, tPipeline); //Use the ADS shader that interpolates between vertices
		batch.Draw(); //Draw the batch with the above specified shader
		
		mvStack.PopMatrix(); //Pop back to the previous matrix, reverting the changes
	}
}

//Loop through every building of the building type, and draw them:
void House::drawAll(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient)
{
	for (unsigned int i = 0 ; i < houses.size() ; i++) //Loop through every position in the vector ...
	{
		houses[i]->draw(emilShaders, tPipeline, vLightPos, mvStack, vAmbient, houseBatch); //... and draw the building at those positions
	}
}

//Create a building - not a building type, a factual building:
void House::create(vector< vector<float> > &pos)
{
	houses.push_back(new House()); //Add a new element to the vector houses, to represent the newly constructet building
	houses[houses.size()-1]->cubes = pos.size(); //Set the number of cubes to the number of positions to draw cubes at
	for (unsigned int i = 0 ; i < pos.size() ; i++) //For every position in the specified vector, ...
	{
		houses[houses.size()-1]->positions.push_back(pos[i]); //... add that position to the new building object
	}
	houses[houses.size()-1]->inherit(this->radius, this->vSpecular, this->vColor); //And finally inherit values from the building type
}

//Used to make buildings inherit values from their building type:
void House::inherit(GLfloat fRadius, M3DVector4f shine, M3DVector4f color)
{
	//Just pass on the values:
	radius = fRadius;
	for (unsigned int i = 0 ; i < 4 ; i++)
	{
		vSpecular[i] = shine[i];
		vColor[i] = color[i];
	}
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

//Returns the positions of the cubes that make up the building
vector< vector< float > > House::getPositions()
{
	return positions;
}

//Returns the radius of the cubes:
float House::getRadius()
{
	return radius;
}

//Returns the buildings of the building type:
vector< House * > House::getHouses()
{
	return houses;
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

