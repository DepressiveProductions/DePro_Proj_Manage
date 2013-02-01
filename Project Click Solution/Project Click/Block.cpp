#include "Block.h"


Block::Block(void) {}

Block::~Block(void) {}

void Block::init(float x, float y, float z, float width, float height)
{
	aFrame.SetOrigin(x, y, z); // x, y, z is the center of the cuboid
	
	w = width/2;
	h = height/2;
	d = 5.0f/2;

	vColor[0] = 0.5f;
	vColor[1] = 0.5f;
	vColor[2] = 0.5f;
	vColor[3] = 1.0f;

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
	aBatch.Vertex3f(-w, h, d);
	aBatch.Vertex3f(w, h, d);
	aBatch.Vertex3f(-w, -h, d);
	aBatch.Vertex3f(w, -h, d);
	aBatch.Vertex3f(-w, -h, -d);
	aBatch.Vertex3f(w, -h, -d);
	aBatch.Vertex3f(-w, h, -d);
	aBatch.Vertex3f(w, h, -d);
	aBatch.Vertex3f(-w, h, d);
	aBatch.Vertex3f(w, h, d);
	aBatch.Vertex3f(w, h, -d);
	aBatch.Vertex3f(w, -h, d);
	aBatch.Vertex3f(w, -h, -d);
	aBatch.Vertex3f(-w, -h, -d);
	aBatch.Vertex3f(-w, h, d);
	aBatch.Vertex3f(-w, h, -d);
	aBatch.Vertex3f(-w, h, d);
	aBatch.End();
}

void Block::draw(GLShaderManager &sManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack, M3DVector4f vLight)
{
	mvStack.PushMatrix();
	mvStack.MultMatrix(aFrame);
	sManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, tPipeline.GetModelViewMatrix(), tPipeline.GetProjectionMatrix(), vLight, vColor);
	aBatch.Draw();
	mvStack.PopMatrix();
}