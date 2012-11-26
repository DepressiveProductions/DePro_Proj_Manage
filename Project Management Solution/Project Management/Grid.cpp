#include "Grid.h"

Grid::Grid() {}

void Grid::init(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat Scale) // Initialize grid-square. XY is center of square.
{
	// Center of floor (0.0f, 0.0f, 0.0f)
	center[0] = centerX;
	center[1] = centerY;
	center[2] = centerZ;

	gridScale = Scale;
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
	int modX = 1;
	int counterX = 0;
	GLfloat xDiff = pos[0]-center[0];
	if (xDiff >= 0) modX = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modX*xDiff > 0.0f; xDiff += modX*gridScale*2)
		counterX++;
	
	int modY = 1;
	int counterY = 0;
	GLfloat yDiff = pos[1]-center[1];
	if (yDiff >= 0) modY = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modY*yDiff > 0.0f; yDiff += modY*gridScale*2)
		counterY++;

	pos[0] = -modX*(counterX-1)*gridScale*2 + -modX*gridScale;
	pos[1] = -modY*(counterY-1)*gridScale*2 + -modY*gridScale;
	pos[2] = 0.0f;
}

void Grid::activateSquare(M3DVector3f &pos)
{
	M3DVector3f inPos; // To prevent changes of original pos
	for (int i=0; i < 3; i++)
		inPos[i] = pos[i];
	calculateExactPos(inPos);

	// Loop through all squares and return the right one
	hl.push_back(new highlighted);
	GLfloat vSquare[] = {inPos[0]-gridScale, inPos[1]-gridScale, inPos[2],
						 inPos[0]-gridScale, inPos[1]+gridScale, inPos[2],
						 inPos[0]+gridScale, inPos[1]+gridScale, inPos[2],
						 inPos[0]+gridScale, inPos[1]-gridScale, inPos[2]};
	hl[hl.size()-1]->batch.Begin(GL_LINE_LOOP, 4);
	hl[hl.size()-1]->batch.CopyVertexData3f(vSquare);
	hl[hl.size()-1]->batch.End();

	for (int i=0; i < 3; i++)
		hl[hl.size()-1]->pos[i] = inPos[i];
}

void Grid::boxActivation(M3DVector3f &startPos, M3DVector3f &endPos)
{
	M3DVector3f sPos;
	M3DVector3f ePos;
	for (int i=0; i < 3; i++)
	{
		sPos[i] = startPos[i];
		ePos[i] = endPos[i];
	}
	M3DVector3f squarePos;
	squarePos[2] = 0.0f;
	int xDiff, yDiff; // Number of squares in x- and y-axis
	calculateExactPos(sPos);
	calculateExactPos(ePos);

	int modX = 1;
	int	modY = 1;
	if (sPos[0] >= ePos[0]) modX = -1;
	if (sPos[1] >= ePos[1]) modY = -1;

	xDiff = abs((sPos[0] - ePos[0])/gridScale - modX);
	yDiff = abs((sPos[1] - ePos[1])/gridScale - modY);

	// Activate all squares
	for (int y=0; y < yDiff; y++)
	{
		squarePos[1] = sPos[1]+(modY *y*gridScale);
		for (int x=0; x < xDiff; x++)
		{
			squarePos[0] = sPos[0]+(modX*x*gridScale);
			activateSquare(squarePos);
		}
	}
}

void Grid::deactivateAllSquares()
{
	while (hl.size() > 0)
		hl.erase(hl.begin()); // Erase first element
}

void Grid::getSquarePosition(vector< M3DVector3f > &positions)
{
	for (unsigned int i=0; i < hl.size(); i++)
	{
		M3DVector3f pos;
		pos[0] = hl[i]->pos[0];
		pos[1] = hl[i]->pos[1];
		pos[2] = hl[i]->pos[2];
		positions.push_back(pos);
	}
}

void Grid::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	for (unsigned int i=0; i < hl.size(); i++)
	{
		glDisable(GL_DEPTH_TEST);
		glLineWidth(lineWidth);
		mvStack.PushMatrix();
		shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
		hl[i]->batch.Draw();
		mvStack.PopMatrix();
		glLineWidth(1.0f); // Standard width
		glEnable(GL_DEPTH_TEST);
	}
}