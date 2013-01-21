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
		void setColor(M3DVector4f vNewColor);
		void setColor(array<GLfloat,4> vNewColor);
		void setPositions(vector< array<float,3> > pos);
		void setPositions(array<float,3> pos1, array<float,3> pos2, array<float,3> pos3, array<float,3> pos4);

		//Getters:

		vector< array<float,3> > getPositions();

	private:
		float width, height, z;
		vector<array<float,3>> positions;
		M3DVector4f vColor;
		GLBatch bgBatch;
};

////////////////////////////////////////////////////////////////////////////////////////// End of class