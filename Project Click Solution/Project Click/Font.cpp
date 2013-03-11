#include "Font.h"

Font::Font(void) {}
Font::~Font(void) {}

void Font::init()
{
	
}



void Font::showText(std::string text, float x, float y, float width, float height)
{
	GLBatch tempBatch;
	tempBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	tempBatch.Vertex3f(xmin, ymin, z);

	// Upper left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	tempBatch.Vertex3f(xmin, ymax, z);  

	// Upper right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	tempBatch.Vertex3f(xmax, ymax, z);

	// Lower right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	tempBatch.Vertex3f(xmax, ymin, z);

	tempBatch.End();
	
}

void Font::clearTexture()
{
	//glDeleteTextures(1, uiTexture);
}