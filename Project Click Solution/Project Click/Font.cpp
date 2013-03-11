#include "Font.h"

Font::Font(void) {}
Font::~Font(void) {}

void Font::init()
{
	vColor[0] = 0.0f;
	vColor[1] = 0.0f;
	vColor[2] = 1.0f;
	vColor[3] = 1.0f;
}



void Font::showText(std::string text, float x, float y,
					float width, float height, GLShaderManager &sm,
					GLGeometryTransform tp)
{
	float z = 0.0f;

	GLBatch tempBatch;
	tempBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	tempBatch.Vertex3f(x, y, z);

	// Upper left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	tempBatch.Vertex3f(x, y + height, z);  

	// Upper right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	tempBatch.Vertex3f(x + width, y + height, z);

	// Lower right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	tempBatch.Vertex3f(x + width, y, z);

	tempBatch.End();

	draw(tempBatch, sm, tp);
}

void Font::draw(GLBatch &ba, GLShaderManager &sm, GLGeometryTransform tp)
{
	sm.UseStockShader(GLT_SHADER_FLAT, tp.GetModelViewProjectionMatrix(), vColor);
	ba.Draw();
}

void Font::clearTexture()
{
	//glDeleteTextures(1, uiTexture);
}