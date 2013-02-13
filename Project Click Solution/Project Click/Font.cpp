#include "Font.h"


Font::Font(void) {}
Font::~Font(void) {}

void Font::init(map<char,std::string> texFileNames)
{
	nTextures = texFileNames.size();

	glGenTextures(nTextures, uiTexture);
	int counter = 0;
	for (auto i = texFileNames.begin() ; i != texFileNames.end(); i++) {
		glBindTexture(GL_TEXTURE_2D, uiTexture[counter]);
		loadTGATexture(i->second.c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		texIDs[i->first] = counter;
		counter++;
	}
}

void Font::showText(std::string text, float x, float y, float width, float height)
{
	if (height == 0.0f) {										//Default aspect ratio
		height = width * 5 / 3;
	} if (x + (width * ((float)text.size() + 1.0f)) > 100.0f) {	//Not outside screen!
		x =  -(width * ((float)text.size() + 1.0f)) - 100.0f;
	}

	vector<letter *> tempVec;

	for (unsigned int i = 0.0f ; i < text.size() ; i++) {
		tempVec.push_back(new letter);
		initiateBatch(tempVec[tempVec.size()-1], x + i*width, y, width, height);
		tempVec[tempVec.size()-1]->iTexture = texIDs[text[i]];
		tempVec[tempVec.size()-1]->c = text[i];
		allText[text] = tempVec;
	}
}

void Font::initiateBatch(letter *l, float x, float y, float width, float height)
{
	l->lBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	z = 0.0f;

	// Lower left hand corner
	l->lBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	l->lBatch.Vertex3f(x, y, z);

	// Upper left hand corner
	l->lBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	l->lBatch.Vertex3f(x, y + height, z);  

	// Upper right hand corner
	l->lBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	l->lBatch.Vertex3f(x + width, y + height, z);

	// Lower right hand corner
	l->lBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	l->lBatch.Vertex3f(x + width, y, z);

	l->lBatch.End();
}

void Font::drawAll(GLGeometryTransform pipeline, GLShaderManager &shaderManager)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	for (auto iter = allText.begin() ; iter != allText.end() ; iter++) {

		for (unsigned int i = 0 ; i < iter->second.size() ; i++) {
			glBindTexture(GL_TEXTURE_2D, uiTexture[iter->second[i]->iTexture]);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, pipeline.GetModelViewProjectionMatrix(), iter->second[i]->iTexture);
			iter->second[i]->lBatch.Draw();
		}
	}

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
	glDeleteTextures(nTextures, uiTexture);
}