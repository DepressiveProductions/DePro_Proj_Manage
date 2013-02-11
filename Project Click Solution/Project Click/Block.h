#pragma once
#include "Actor.h"
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <ctime>
#include "Shaders.h"

class Block : private Actor
{
public:
	Block(void);
	~Block(void);
	void init(float x, float y, float z, float width, float height, float moveSpeed);
	void setColor(float r, float g, float b, float a);
	void move();
	bool isWithin(float x, float y, float z);
	void draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient);

private:
	// x, y, z stored in actor-frame
	float w, h, d;
	float mvSpeed;
	float lastTick;

	void generateBatch();
};