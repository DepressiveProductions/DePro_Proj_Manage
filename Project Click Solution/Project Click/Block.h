#pragma once
#include "Actor.h"
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLShaderManager.h>

class Block : private Actor
{
public:
	Block(void);
	~Block(void);
	void init(float x, float y, float z, float width, float height);
	void setColor(float r, float g, float b, float a);
	void draw(GLShaderManager &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	// x, y, z stored in actor-frame
	float w, h, d;

	void generateBatch();
};

