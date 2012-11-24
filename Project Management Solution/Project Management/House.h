#pragma once
#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

class House
{
	public:
		House(void);
		~House(void);
		
		void init(GLfloat fRadius);
		void create(M3DVector4f vStartingPos, M3DVector4f vEndingPos, int iCubes);
		void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack);
		void nextPos();

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		
		void getColor(M3DVector4f &vOut);
		void getPositions(M3DVector4f &vOutStarting, M3DVector4f &vOutEnding);
		float getRadius();
	
	private:
		M3DVector4f vColor;
		M3DVector4f vStartingPos;
		M3DVector4f vEndingPos;
		M3DVector3f vCurrentPos;
		
		GLBatch houseBatch;
		GLFrame houseFrame;
		
		GLfloat radius;
		int cubes;
};

