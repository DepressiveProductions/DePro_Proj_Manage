#include "Block.h"


Block::Block(void) {}

Block::~Block(void) {}

void Block::init(float x, float y, float z, float width, float height)
{
	aFrame.SetOrigin(x, y, z+1.0f); // x, y, z is the center of the back of the cuboid
	
	w = width/2;
	h = height/2;
	d = 1.0f;

	vColor[0] = 0.8f;
	vColor[1] = 0.8f;
	vColor[2] = 0.8f;
	vColor[3] = 1.0f;

	vShininess[0] = 0.4f;
	vShininess[1] = 0.4f;
	vShininess[2] = 0.4f;
	vShininess[3] = 1.0f;

	generateBatch();
}

void Block::setColor(float r, float g, float b, float a)
{
	vColor[0] = r;
	vColor[1] = g;
	vColor[2] = b;
	vColor[3] = a;
}

void Block::generateBatch()
{
	aBatch.Begin(GL_TRIANGLE_STRIP, 17); //Why not use CopyVertexData3f like before?
	aBatch.Vertex3f(-w, h, 1.0f);
	aBatch.Vertex3f(w, h, 1.0f);
	aBatch.Vertex3f(-w, -h, 1.0f);
	aBatch.Vertex3f(w, -h, 1.0f);
	aBatch.Vertex3f(-w, -h, 0.0f);
	aBatch.Vertex3f(w, -h, 0.0f);
	aBatch.Vertex3f(-w, h, 0.0f);
	aBatch.Vertex3f(w, h, 0.0f);
	aBatch.Vertex3f(-w, h, 1.0f);
	aBatch.Vertex3f(w, h, 1.0f);
	aBatch.Vertex3f(w, h, 0.0f);
	aBatch.Vertex3f(w, -h, 1.0f);
	aBatch.Vertex3f(w, -h, 0.0f);
	aBatch.Vertex3f(-w, -h, 0.0f);
	aBatch.Vertex3f(-w, h, 1.0f);
	aBatch.Vertex3f(-w, h, 0.0f);
	aBatch.Vertex3f(-w, h, 1.0f);
	aBatch.End();
}

void Block::draw(Shaders &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight, M3DVector4f vAmbient)
{
	mvStack.PushMatrix();
	mvStack.MultMatrix(aFrame);
	sManager.useADSVert(vColor, vAmbient, vShininess, vLight, tPipeline);
	aBatch.Draw();
	mvStack.PopMatrix();
}