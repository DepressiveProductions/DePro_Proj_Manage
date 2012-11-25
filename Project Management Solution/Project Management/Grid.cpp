#include "Grid.h"

Grid::Grid() {}

void Grid::init(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat Scale) // Initialize grid-square. XY is center of square.
{
	// Center of floor (0.0f, 0.0f, 0.0f)
	center[0] = centerX;
	center[1] = centerY;
	center[2] = centerZ;

	gridScale = Scale/2;
	lineWidth = 2.0f;
	setColour(0.4f, 0.4f, 0.4f, 0.8f);
}

void Grid::setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vGridColour[0] = r;
	vGridColour[1] = g;
	vGridColour[2] = b;
	vGridColour[3] = a;
}

void Grid::setLineWidth(GLfloat w)
{
	lineWidth = w;
}

void Grid::calculateExactPos(M3DVector3f &pos)
{
	// Conter is the amount of gridsquares that differ between (click)pos and centerpos
	int counterX = 0;
	GLfloat xDiff = pos[0]-center[0];
	for (; xDiff > 0.0f; xDiff -= gridScale*2)
		counterX++;
	
	int counterY = 0;
	GLfloat yDiff = pos[1]-center[1];
	for (; yDiff > 0.0f; yDiff -= gridScale*2)
		counterY++;
	pos[0] = counterX*gridScale*2 + gridScale;
	pos[1] = counterY*gridScale*2 + gridScale;
}

void Grid::addSquare(M3DVector3f &pos)
{
	calculateExactPos(pos);
	// Loop through all squares and return the right one
	for (unsigned int i=0; i < hl.size(); i++)
	{
		if ((hl[i]->pos[0] == pos[0]) && (hl[i]->pos[1] == pos[1]) && (hl[i]->pos[2] == pos[2]))
			highlightSquare(pos);
	}
}

void Grid::removeSquare(M3DVector3f &pos)
{
	calculateExactPos(pos);
	// Loop through all squares and return the right one
	for (unsigned int i=0; i < hl.size(); i++)
	{
		if ((hl[i]->pos[0] == pos[0]) && (hl[i]->pos[1] == pos[1]) && (hl[i]->pos[2] == pos[2]))
			hl.erase(hl.begin() + i);
	}
}

void Grid::removeAllSquares()
{
	while (hl.size() > 0)
		hl.erase(hl.begin());
}

void Grid::highlightSquare(M3DVector3f pos)
{
	hl.push_back(new highlighted);
	GLfloat vSquare[] = {pos[0]-gridScale, pos[1]-gridScale, pos[2],
						 pos[0]-gridScale, pos[1]+gridScale, pos[2],
						 pos[0]+gridScale, pos[1]+gridScale, pos[2],
						 pos[0]+gridScale, pos[1]-gridScale, pos[2]};
	hl[hl.size()-1]->batch.Begin(GL_LINE_LOOP, 4);
	hl[hl.size()-1]->batch.CopyVertexData3f(vSquare);
	hl[hl.size()-1]->batch.End();

	for (int i=0; i < 3; i++)
		hl[hl.size()-1]->pos[i] = pos[i];
}

void Grid::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	for (unsigned int i=0; i < hlBatch.size(); i++)
	{
		glLineWidth(lineWidth);
		mvStack.PushMatrix();
		shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
		hlBatch[i]->Draw();
		mvStack.PopMatrix();
		glLineWidth(1.0f); // Standard width
	}
}