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

void Grid::calculateExactPos(M3DVector3f &pos) // Sets pos to nearest grid-square postion
{
	int modX = 1; // Modifier for +/- delta x, will be 1 or -1
	int counterX = 0; // Is the amount of gridsquares that differ between (click)pos and centerpos
	GLfloat xDiff = pos[0]-center[0];
	if (xDiff >= 0) modX = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modX*xDiff > 0.0f; xDiff += modX*gridScale*2)
		counterX++;
	
	int modY = 1; // Modifier for +/- delta x, will be 1 or -1
	int counterY = 0; // Is the amount of gridsquares that differ between (click)pos and centerpos
	GLfloat yDiff = pos[1]-center[1];
	if (yDiff >= 0) modY = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modY*yDiff > 0.0f; yDiff += modY*gridScale*2)
		counterY++;

	pos[0] = -modX*(counterX-1)*gridScale*2 + -modX*gridScale;
	pos[1] = -modY*(counterY-1)*gridScale*2 + -modY*gridScale;
}
/*
void Grid::activateSquare(M3DVector3f &pos)
{
	M3DVector3f inPos;
	for (int i=0; i<3; i++)
		inPos[i] = pos[i];
	calculateExactPos(inPos);

	// Create a batch for the highlighted square
	hl.push_back(new highlighted);
	GLfloat vSquare[] = {inPos[0]-gridScale, inPos[1]-gridScale, inPos[2],
						 inPos[0]-gridScale, inPos[1]+gridScale, inPos[2],
						 inPos[0]+gridScale, inPos[1]+gridScale, inPos[2],
						 inPos[0]+gridScale, inPos[1]-gridScale, inPos[2]};
	hl[hl.size()-1]->batch.Begin(GL_LINE_LOOP, 4);
	hl[hl.size()-1]->batch.CopyVertexData3f(vSquare);
	hl[hl.size()-1]->batch.End();

	// Add the squares position to the hl-struct
	for (int i=0; i < 3; i++)
		hl[hl.size()-1]->pos[i] = inPos[i];
}*/

void Grid::activateSquares()
{
	M3DVector3f upperLeftCorner;
	M3DVector3f lowerRightCorner;
	
	for (int i=0; i<2; i++)
	{
		upperLeftCorner[i] = squarePositions[0][i]-gridScale;
		lowerRightCorner[i] = squarePositions[squarePositions.size()-1][i]+gridScale;
	}
	upperLeftCorner[2] = squarePositions[0][2];
	lowerRightCorner[2] = squarePositions[squarePositions.size()-1][2];

	hlBatch.Begin(GL_LINES, 20*(((lowerRightCorner[0]-upperLeftCorner[0])/gridScale)+((lowerRightCorner[1]-upperLeftCorner[1])/gridScale) + 2));
	// Lines in Y-axis
	for (GLfloat x = upperLeftCorner[0]; x <= lowerRightCorner[0]; x += 2*gridScale)
	{
		hlBatch.Vertex3f(x, upperLeftCorner[1], upperLeftCorner[2]);
		hlBatch.Vertex3f(x, lowerRightCorner[1], lowerRightCorner[2]);
	}
	// Lines in X-axis
	for (GLfloat y = upperLeftCorner[1]; y <= lowerRightCorner[1]; y += 2*gridScale)
	{
		hlBatch.Vertex3f(upperLeftCorner[0], y, upperLeftCorner[2]);
		hlBatch.Vertex3f(lowerRightCorner[0], y, lowerRightCorner[2]);
	}
	hlBatch.End();
}

void Grid::setUpperLeft(M3DVector3f &sPos, M3DVector3f &ePos)
{
	// Set sPos upper left corner and ePos lower right corner
	GLfloat xUL, yUL, xLR, yLR;
	if (sPos[0] >= ePos[0] && sPos[1] >= ePos[1])
	{	
		xUL = ePos[0];
		yUL = sPos[1];
		xLR = ePos[1];
		yLR = sPos[0];
	}
	else if (sPos[0] >= ePos[0] && sPos[1] < ePos[1])
	{
		xUL = ePos[0];
		yUL = ePos[1];
		xLR = sPos[0];
		yLR = sPos[1];
	}
	else if (sPos[0] < ePos[0] && sPos[1] < ePos[1])
	{
		xUL = sPos[0];
		yUL = ePos[1];
		xLR = ePos[0];
		yLR = sPos[1];
	}
	else if (sPos[0] < ePos[0] && sPos[1] >= ePos[1])
	{
		xUL = sPos[0];
		yUL = sPos[1];
		xLR = ePos[0];
		yLR = ePos[1];
	}
	sPos[0] = xUL;
	sPos[1] = yUL;
	ePos[0] = xLR;
	ePos[1] = yLR;
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

	// Batch with lines forming a rectangle from clicking position to actual position
	GLfloat vVerts[] = {sPos[0], sPos[1], center[2],
						ePos[0], sPos[1], center[2],
						ePos[0], ePos[1], center[2],
						sPos[0], ePos[1], center[2]};
	markingLines.Begin(GL_LINE_LOOP, 4);
	markingLines.CopyVertexData3f(vVerts);
	markingLines.End();

	vector<float> squarePos;
	squarePos.resize(3);
	int xDiff, yDiff; // Number of squares in x- and y-axis
	calculateExactPos(sPos);
	calculateExactPos(ePos);
	setUpperLeft(sPos, ePos);

	int modX = 1; // Modifier for +/- delta, will be 1 or -1
	int	modY = 1; // Modifier for +/- delta, will be 1 or -1

	xDiff = abs((sPos[0] - ePos[0])/gridScale - 1);
	yDiff = abs((sPos[1] - ePos[1])/gridScale + 1);

	// Activate all squares
	for (int y=0; y < yDiff; y++)
	{
		squarePos[1] = sPos[1]+(-1*y*gridScale);
		for (int x=0; x < xDiff; x++)
		{
			squarePos[0] = sPos[0]+(x*gridScale);
			squarePositions.push_back(squarePos); // Storage for later use
		}
	}
	activateSquares();
}

void Grid::deactivateAllSquares()
{
	squarePositions.clear();
	hlBatch.Begin(GL_LINES, 1);
	hlBatch.Vertex3f(0.0f, 0.0f, 0.0f);
	hlBatch.End();
}

void Grid::getSquarePositions(vector< vector<float> > &positions) // "Returns" all highlighted square-positions
{
	for (unsigned int i=0; i < squarePositions.size(); i++)
	{
		vector<float> pos;
		pos.push_back(squarePositions[i][0]);
		pos.push_back(squarePositions[i][1]);
		pos.push_back(squarePositions[i][2]);
		positions.push_back(pos);
	}
}

void Grid::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	glDisable(GL_DEPTH_TEST); // Draw it on top of everything
	glLineWidth(lineWidth);
	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
	hlBatch.Draw();
	mvStack.PopMatrix();
	glLineWidth(1.0f); // Standard width

	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
	markingLines.Draw();
	mvStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}