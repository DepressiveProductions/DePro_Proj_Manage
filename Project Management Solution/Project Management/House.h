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

class House
{
	public:
		House(void);
		~House(void);
		
		void init(GLfloat fRadius); //Initiate a building type with default shinyness and color
		void init(GLfloat fRadius, M3DVector4f shine, M3DVector4f color); //Initiate a building with specified shinyness and color
		void create(vector< vector<float> > &pos); //Create a building - not a building type, a factual building
		void draw(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient, GLBatch &batch); //Draw building
		void drawAll(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient); //Loop through every building of the building type, and draw them
		void inherit(GLfloat fRadius, M3DVector4f shine, M3DVector4f color); //Used to make buildings inherit values from their building type

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a); //Set building type color to specified GLfloats
		void setColor(M3DVector4f &vOldColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a); //Set a specified color to specified GLfloats
		void setColor(M3DVector4f &vOldColor, M3DVector4f vNewColor); //Set specified color to specified M3DVector4f
		
		void getColor(M3DVector4f &vOut); //Returns the building type color by setting it to &vOut
		float getRadius(); //Returns the radius of the cubes
		vector< vector<float> > getPositions(); //Returns the positions of the cubes that make up the building
		vector< House * > getHouses(); //Returns the buildings of the building type
	
	private:
		M3DVector4f vColor; //Color of the building

		M3DVector4f vSpecular; //Shinyness of the building

		GLBatch houseBatch; //Batch to draw
		GLFrame houseFrame; //Frame used in the drawing loop

		vector< House * > houses; //List of buildings of the building type
		vector< vector< float > > positions; //List of positions of the cubes that make up the building
		
		GLfloat radius; //Radius of cubes that make up the buildings
		unsigned int cubes; //Used in the drawing method, represents number of cubes that make up a building
};

