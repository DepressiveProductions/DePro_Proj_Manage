#pragma once
#include "Actor.h"
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include "Shaders.h"

class Block : private Actor
{
public:
	Block(void);
	~Block(void);
	void init(float x, float y, float z, float width, float height);
	void setColor(float r, float g, float b, float a);
	void move();
	void draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient);

private:
	// x, y, z stored in actor-frame
	float w, h, d;

	void generateBatch();
};