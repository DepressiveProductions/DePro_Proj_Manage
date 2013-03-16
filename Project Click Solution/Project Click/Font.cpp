#include "Font.h"

Font::Font(void) {}
Font::~Font(void) {}

void Font::init(std::string fontFileName)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	loadTGATexture(fontFileName.c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	
	vColor[0] = 0.0f;
	vColor[1] = 0.0f;
	vColor[2] = 1.0f;
	vColor[3] = 1.0f;
}



void Font::showText(char *text, float x, float y,
					float width, float height, GLShaderManager &sm,
					GLGeometryTransform tp)
{
	float z = 0.0f;

	float glyphWidth = width/strlen(text);

	glBindTexture(GL_TEXTURE_2D, uiTexture);

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
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	sm.UseStockShader(GLT_SHADER_FLAT, tp.GetModelViewProjectionMatrix(), vColor);
	//sm.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, tp.GetModelViewProjectionMatrix(), 0);
	ba.Draw();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

bool Font::loadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return true;
}

void Font::clearTexture()
{
	//glDeleteTextures(1, uiTexture);
}