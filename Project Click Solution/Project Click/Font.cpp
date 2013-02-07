#include "Font.h"


Font::Font(void) {}
Font::~Font(void) {}

void Font::init(float xmin, float ymin, float xmax, float ymax, float z, vector<std::string> texFileNames)
{
	this->xmin = xmin;
	this->ymin = ymin;
	this->xmax = xmax;
	this->ymax = ymax;

	this->z = z;

	nTextures = texFileNames.size();

	//uiFrame.SetOrigin(x, y, z);

	initiateBatch();

	glGenTextures(nTextures, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	for (unsigned int i = 0 ; i < nTextures ; i++) {
		loadTGATexture(texFileNames[i].c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
	}
}

void Font::initiateBatch()
{
	rectBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	rectBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	rectBatch.Vertex3f(xmin, ymin, z);

	// Upper left hand corner
	rectBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	rectBatch.Vertex3f(xmin, ymax, z);  

	// Upper right hand corner
	rectBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	rectBatch.Vertex3f(xmax, ymax, z);

	// Lower right hand corner
	rectBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	rectBatch.Vertex3f(xmax, ymin, z);

	rectBatch.End();
}

void Font::draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, pipeline.GetModelViewProjectionMatrix(), 0);
	rectBatch.Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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
	glDeleteTextures(nTextures, &uiTexture);
}