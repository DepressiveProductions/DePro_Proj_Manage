#include "Grid.h"

Grid::Grid() {}

void Grid::init(float centerX, float centerY, float centerZ, float Scale) // Initialize grid-square. XY is center of square.
{
	// Center of floor (0.0f, 0.0f, 0.0f)
	center[0] = centerX;
	center[1] = centerY;
	center[2] = centerZ;

	gridScale = Scale;
	lineWidth = 2.0f;
	setColour(0.4f, 0.4f, 0.4f, 0.8f);
}

void Grid::setColour(float r, float g, float b, float a)
{
	vGridColour[0] = r;
	vGridColour[1] = g;
	vGridColour[2] = b;
	vGridColour[3] = a;
}

void Grid::setLineWidth(float w)
{
	lineWidth = w;
}

void Grid::calculateExactPos(array<float, 3> &pos) // Sets pos to nearest grid-square postion
{
	int modX = 1; // Modifier for +/- delta x, will be 1 or -1
	int counterX = 0; // Is the amount of gridsquares that differ between (click)pos and centerpos
	float xDiff = pos[0]-center[0];
	if (xDiff >= 0) modX = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modX*xDiff > 0.0f; xDiff += modX*gridScale*2)
		counterX++;
	
	int modY = 1; // Modifier for +/- delta x, will be 1 or -1
	int counterY = 0; // Is the amount of gridsquares that differ between (click)pos and centerpos
	float yDiff = pos[1]-center[1];
	if (yDiff >= 0) modY = -1; // The modifier always makes the loop go -> 0.0f
	for (; -modY*yDiff > 0.0f; yDiff += modY*gridScale*2)
		counterY++;

	pos[0] = -modX*(counterX-1)*gridScale*2 + -modX*gridScale;
	pos[1] = -modY*(counterY-1)*gridScale*2 + -modY*gridScale;
}

void Grid::activateSquares()
{
	array<float, 3> upperLeftCorner;
	array<float, 3> lowerRightCorner;
	
	upperLeftCorner[0] = squarePositions[0][0]-gridScale;
	upperLeftCorner[1] = squarePositions[0][1]+gridScale;
	lowerRightCorner[0] = squarePositions[squarePositions.size()-1][0]+gridScale;
	lowerRightCorner[1] = squarePositions[squarePositions.size()-1][1]-gridScale;

	upperLeftCorner[2] = squarePositions[0][2];
	lowerRightCorner[2] = squarePositions[squarePositions.size()-1][2];

	hlBatch.Begin(GL_LINES, 4+(((lowerRightCorner[0]-upperLeftCorner[0])/gridScale)+((upperLeftCorner[1]-lowerRightCorner[1])/gridScale)));
	// Lines in Y-axis
	for (GLfloat x = upperLeftCorner[0]; x <= lowerRightCorner[0]; x += 2*gridScale)
	{
		hlBatch.Vertex3f(x, upperLeftCorner[1], 0.0f);
		hlBatch.Vertex3f(x, lowerRightCorner[1], 0.0f);
	}
	// Lines in X-axis
	for (GLfloat y = upperLeftCorner[1]; y >= lowerRightCorner[1]; y -= 2*gridScale)
	{
		hlBatch.Vertex3f(upperLeftCorner[0], y, 0.0f);
		hlBatch.Vertex3f(lowerRightCorner[0], y, 0.0f);
	}
	hlBatch.End();
}

void Grid::setUpperLeft(array<float, 3> &sPos, array<float, 3> &ePos)
{
	// Set sPos upper left corner and ePos lower right corner
	float xUL, yUL, xLR, yLR;
	if (sPos[0] >= ePos[0] && sPos[1] >= ePos[1])
	{	
		xUL = ePos[0];
		yUL = sPos[1];
		xLR = sPos[0];
		yLR = ePos[1];
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

void Grid::boxActivation(array<float, 3> &startPos, array<float, 3> &endPos)
{
	array<float, 3> sPos;
	array<float, 3> ePos;
	for (int i=0; i < 3; i++)
	{
		sPos[i] = startPos[i];
		ePos[i] = endPos[i];
	}

	// Batch with lines forming a rectangle from clicking position to actual position
	float vVerts[] = {sPos[0], sPos[1], center[2],
						ePos[0], sPos[1], center[2],
						ePos[0], ePos[1], center[2],
						sPos[0], ePos[1], center[2]};
	markingLines.Begin(GL_LINE_LOOP, 4);
	markingLines.CopyVertexData3f(vVerts);
	markingLines.End();

	array<float, 3> squarePos;
	int xDiff, yDiff; // Number of squares in x- and y-axis
	calculateExactPos(sPos);
	calculateExactPos(ePos);
	setUpperLeft(sPos, ePos);

	xDiff = abs((sPos[0] - ePos[0])/gridScale) + 1;
	yDiff = abs((sPos[1] - ePos[1])/gridScale) + 1;

	// Activate all squares
	for (int y=0; y < yDiff; y++)
	{
		squarePos[1] = sPos[1]-(y*gridScale);
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
	hlBatch.Reset();
	markingLines.Reset();
}

void Grid::getSquarePositions(vector<array<float,3>> &positions) // "Returns" all highlighted square-positions
{
	for (unsigned int i=0; i < squarePositions.size(); i++)
	{
		array<float,3> pos;
		pos[0] = squarePositions[i][0];
		pos[1] = squarePositions[i][1];
		pos[2] = 0.0f;
		positions.push_back(pos);
	}
}

void Grid::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	glDisable(GL_DEPTH_TEST); // Draw it on top of everything
	glLineWidth(lineWidth); // Set line width
	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
	hlBatch.Draw(); // Draw HL-grid
	mvStack.PopMatrix();
	glLineWidth(1.0f); // Standard width

	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
	markingLines.Draw(); // Draw selection-box
	mvStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}