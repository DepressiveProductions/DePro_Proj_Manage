#pragma once
#include <GLTools.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include "MyShaderManager.h"

class House
{
	public:
		House(void);
		~House(void);
		
		void init(GLfloat fRadius);
		void init(GLfloat fRadius, M3DVector4f shine, M3DVector4f color);
		void create(M3DVector4f vStartingPos, M3DVector4f vEndingPos, int iCubes);
		void draw(MyShaderManager &emilShaders, GLGeometryTransform &tPipeline, M3DVector4f vLightPos, GLMatrixStack &mvStack, M3DVector4f vAmbient);
		void nextPos();

		void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		void setColor(M3DVector4f &vCustColor, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		void setColor(M3DVector4f &vOldColor, M3DVector4f vNewColor);
		
		void getColor(M3DVector4f &vOut);
		void getPositions(M3DVector4f &vOutStarting, M3DVector4f &vOutEnding);
		float getRadius();
	
	private:
		M3DVector4f vColor;

		M3DVector4f vSpecular;

		M3DVector4f vStartingPos;
		M3DVector4f vEndingPos;
		M3DVector3f vCurrentPos;
		
		GLBatch houseBatch;
		GLFrame houseFrame;
		
		GLfloat radius;
		int cubes;
};

