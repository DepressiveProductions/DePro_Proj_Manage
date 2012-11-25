#include "Floor.h"
#include <vector>

using std::vector;

// Publics
GLBatch fBatch;
M3DVector3f vFloorColour;
Grid grid;
vector<Grid *> hlGrid;

// Privates
GLfloat width;
GLfloat height;
GLfloat scale;

Floor::Floor() {}

// Width and Height is number of grid-squares in X- and Y-axis
void Floor::init(int Width, int Height, GLfloat Scale)
{
	width = (GLfloat)(Width*Scale);
	height = (GLfloat)(Height*Scale);
	scale = Scale;
}

void Floor::setColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vFloorColour[0] = r;
	vFloorColour[1] = g;
	vFloorColour[2] = b;
	vFloorColour[3] = a;
}

void Floor::generate()
{

	GLfloat vVerts[] = {-width, -height, 0.0f,
						-width, height, 0.0f,
						width, -height, 0.0f,
						width, height, 0.0f};
	fBatch.Begin(GL_TRIANGLE_STRIP, 4);
	fBatch.CopyVertexData3f(vVerts);
	fBatch.End();

	grid.init(scale, 0.0f, 0.0f, width/scale, height/scale);
	grid.generate();
}

void Floor::addHLSquare(GLfloat x, GLfloat y)
{
	hlGrid.push_back(new Grid());
	hlGrid[hlGrid.size()-1]->init(scale, x, y);
	hlGrid[hlGrid.size()-1]->generate();
}