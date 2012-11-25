#include "Grid.h"

GLBatch gBatch;

GLfloat gridScale; // Half the width or height; square => width = height
GLfloat lineWidth;
GLfloat position[3];
M3DVector4f vGridColour; // Some transparency for full grid, no transparency for small grid 
int squaresX, squaresY;
bool isSquare;

Grid::Grid() {}

void Grid::init(GLfloat Scale, GLfloat X, GLfloat Y) // Initialize grid-square. XY is center of square.
{
	gridScale = Scale/2;
	position[0] = X;
	position[1] = Y;
	position[2] = 0.0f;
	lineWidth = 2.0f;
	setColour(0.8f, 0.8f, 0.8f, 0.4f);
	isSquare = true;
}

void Grid::init(GLfloat Scale, GLfloat X, GLfloat Y, int SquareAmountX, int SquareAmountY) // Initialize grid. XY is center of grid. 
{
	gridScale = Scale/2;
	position[0] = X;
	position[1] = Y;
	position[2] = 0.0f;
	squaresX = SquareAmountX;
	squaresY = SquareAmountY;
	lineWidth = 2.0f;
	setColour(0.8f, 0.8f, 0.8f, 0.4f);
	isSquare = false;
}

void Grid::setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vGridColour[0] = r;
	vGridColour[1] = g;
	vGridColour[2] = b;
	vGridColour[3] = a;
}

void Grid::setLineWidth(GLfloat width)
{
	lineWidth = width;
}

void Grid::generate()
{
	if (isSquare)
	{
		GLfloat vSquare[] = {position[0]-gridScale, position[1]-gridScale, position[2],
							 position[0]-gridScale, position[1]+gridScale, position[2],
							 position[0]+gridScale, position[1]+gridScale, position[2],
							 position[0]+gridScale, position[1]-gridScale, position[2]};
		glLineWidth(lineWidth);
		gBatch.Begin(GL_LINE_LOOP, 4);
		gBatch.CopyVertexData3f(vSquare);
		gBatch.End();
	}
	else if (!isSquare)
	{
		glLineWidth(lineWidth);
		gBatch.Begin(GL_LINES, 10*(squaresX + squaresY + 2));
		// Lines in Y-axis
		for (GLfloat x = position[0] - (2*gridScale*((GLfloat)squaresX)); x <= position[0] + (2*gridScale*(squaresX)); x += 2*gridScale)
		{
			gBatch.Vertex3f(x, -gridScale*2*squaresY, position[2]);
			gBatch.Vertex3f(x, gridScale*2*squaresY, position[2]);
		}
		// Lines in X-axis
		for (GLfloat y = position[1] - (2*gridScale*((GLfloat)squaresY)); y <= position[1] + (2*gridScale*(squaresY)); y += 2*gridScale)
		{
			gBatch.Vertex3f(-gridScale*2*squaresX, y, position[2]);
			gBatch.Vertex3f(gridScale*2*squaresX, y, position[2]);
		}
		gBatch.End();
	}
}