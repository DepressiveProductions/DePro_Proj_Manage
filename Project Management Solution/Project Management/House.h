#pragma once
#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include "MyShaderManager.h"
#include <vector>
#include <array>
using std::array;
using std::vector;

class Block
{
	public:
		Block(void);
		~Block(void);

		void init(array<float,3> pos, float radius);
		void draw(MyShaderManager &shaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, M3DVector4f vColor, M3DVector4f vSpecular, GLBatch &batch);

		void setPosition(array<float,3> pos, float radius);

		array<float,3> getPosition(); //Returns block position
	
private:
		GLFrame blockFrame; //Frame representing position and orientation
		array<float,3> position;
};

class Building
{
	public:
		Building(void);
		~Building(void);
		
		void init(vector< array<float,3> > positions, float radius);
		void drawBlocks(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, M3DVector4f vColor, M3DVector4f vSpecular, GLBatch &batch);

		void setWalls(float x1, float y1, float x2, float y2, float radius); // Upper left- and lower right corner

		vector< array<float,3> > getPositions();
		array<array<float, 3>, 5> getNodes();	//Returns the nodes for  the Path-class
		array<float, 3> getDoorNode();			//Returns the door node
		array<float, 4> getWalls();				//Returns house-borders

	private:
		vector< Block * > blocks;
		array<array<float, 3>, 5> pathNodes;
		array<float, 4> walls;					// left x, right x, lower y, upper y
};


class House
{
	public:
		House(void);
		~House(void);
		
		void init(GLfloat fRadius);															//Initiate a building type with default shinyness and color
		void init(GLfloat fRadius, M3DVector4f shine, M3DVector4f color);					//Initiate a building with specified shinyness and color
		void create(vector< array<float, 3> > pos);											//Create a building - not a building type, a factual building
		void drawAll(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline,
					M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient);	//Loop through every building of the building type, and draw them
		void removeBuilding(unsigned int index);											//Remove a building, index is the index of the buildings-vector

		//Setters:
		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);							//Set building type color to specified GLfloats
		void setColor(M3DVector4f &vOldColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);	//Set a specified color to specified GLfloats
		void setColor(M3DVector4f &vOldColor, M3DVector4f vNewColor);						//Set specified color to specified M3DVector4f
		
		//Getters:
		void getColor(M3DVector4f &vOut);			//Returns the building type color by setting it to &vOut
		float getRadius();							//Returns the radius of the cubes
		vector< Building * > getBuildings();		//Returns the buildings of the building type
		vector< array<float,3> > getPositions();

		//Public attributes:
		vector< Building * > buildings;				//List of buildings of the building type

	private:
		M3DVector4f vColor;							//Color of the building
		M3DVector4f vSpecular;						//Shinyness of the building
		GLBatch houseBatch;							//Batch to draw
		
		GLfloat radius;								//Radius of cubes that make up the buildings
		unsigned int cubes;							//Used in the drawing method, represents number of cubes that make up a building
};

