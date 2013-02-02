#include "UserInterface.h"


UserInterface::UserInterface(void) {}
UserInterface::~UserInterface(void) {}

void UserInterface::init(float x, float y, float z, float width, float height, std::string texFileName)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = width;
	this->h = height;

	//uiFrame.SetOrigin(x, y, z);

	initiateBatch();

	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	loadTGATexture(texFileName.c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
}

void UserInterface::initiateBatch()
{
	rectBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	rectBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	rectBatch.Vertex3f(x, y, 0.0f);

	// Upper left hand corner
	rectBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	rectBatch.Vertex3f(x, y + h, 0.0);  

	// Upper right hand corner
	rectBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	rectBatch.Vertex3f(x + w, y + h, 0.0f);

	// Lower right hand corner
	rectBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	rectBatch.Vertex3f(x + w, y, 0.0f);

	rectBatch.End();
}

void UserInterface::draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, pipeline.GetModelViewProjectionMatrix(), 0);
	rectBatch.Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

bool UserInterface::loadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
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