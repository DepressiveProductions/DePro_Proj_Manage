#pragma once

//GLTools includes:

#include <GLTools.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLShaderManager.h>

//////////////////////////////////////////////////////////////////////////////////////////

//Other includes:

#include <array>
#include <vector>
#include "Actor.h"

//////////////////////////////////////////////////////////////////////////////////////////

using std::array;
using std::vector;

class Background : private Actor
{
public:
	Background(void);
	~Background(void);

	void init(float width, float height);
	void initBatch();
	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline);
		
	//Setters:
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void settings(array<float,3> pos, float width, float height);

	//Getters:


private:
	float width, height, z;
};

////////////////////////////////////////////////////////////////////////////////////////// End of class