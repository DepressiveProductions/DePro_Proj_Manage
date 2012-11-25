#include "Floor.h"

Floor::Floor() {}

// Width and Height is number of grid-squares in X- and Y-axis
void Floor::init(GLfloat X, GLfloat Y, GLfloat Z, int Width, int Height, GLfloat Scale)
{
	position[0] = X;
	position[1] = Y;
	position[2] = Z;

	width = (GLfloat)(Width*Scale);
	height = (GLfloat)(Height*Scale);
	scale = Scale;
	
	// Set colour white
	vFloorColour[0] = 1.0f;
	vFloorColour[1] = 1.0f;
	vFloorColour[2] = 1.0f;
	vFloorColour[3] = 1.0f;
	
	// Set colour green
	vGridColour[0] = 0.8f;
	vGridColour[1] = 0.8f;
	vGridColour[2] = 0.8f;
	vGridColour[3] = 0.4f; // Some transparancy

	// Grid's line width
	gLineWidth = 2.0f;
	
	// Generate floor batch
	GLfloat vVerts[] = {position[0]-(width/scale), position[1]-(height/scale), position[2],
						position[0]-(width/scale), position[1]+(height/scale), position[2],
						position[0]+(width/scale), position[1]-(height/scale), position[2],
						position[0]+(width/scale), position[1]+(height/scale), position[2]};
	fBatch.Begin(GL_TRIANGLE_STRIP, 4);
	fBatch.CopyVertexData3f(vVerts);
	fBatch.End();

	// Generate grid batch
	gBatch.Begin(GL_LINES, 10*(Width + Height + 2));
	// Lines in Y-axis
	for (GLfloat x = position[0] - (2*scale*((GLfloat)Width)); x <= position[0] + (2*scale*(Width)); x += 2*scale)
	{
		gBatch.Vertex3f(x, -scale*2*Height, position[2]);
		gBatch.Vertex3f(x, scale*2*Height, position[2]);
	}
	// Lines in X-axis
	for (GLfloat y = position[1] - (2*scale*((GLfloat)Height)); y <= position[1] + (2*scale*(Height)); y += 2*scale)
	{
		gBatch.Vertex3f(-scale*2*Width, y, position[2]);
		gBatch.Vertex3f(scale*2*Width, y, position[2]);
	}
	gBatch.End();
}

void Floor::setFloorColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vFloorColour[0] = r;
	vFloorColour[1] = g;
	vFloorColour[2] = b;
	vFloorColour[3] = a;
}

void Floor::setGridColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	vGridColour[0] = r;
	vGridColour[1] = g;
	vGridColour[2] = b;
	vGridColour[3] = a;
}

void Floor::setGridLineWidth(GLfloat w)
{
	gLineWidth = w;
}

void Floor::toggleGrid()
{
	drawGrid = !drawGrid;
}

void Floor::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	// Draw floor
	glPolygonOffset(1.0f, 1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);
	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vFloorColour);
	fBatch.Draw();
	mvStack.PopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL);

	// Draw grid
	if (drawGrid)
	{
		glLineWidth(gLineWidth);
		mvStack.PushMatrix();
		shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vGridColour);
		gBatch.Draw();
		mvStack.PopMatrix();
		glLineWidth(1.0f); // Reset standard width
	}
}