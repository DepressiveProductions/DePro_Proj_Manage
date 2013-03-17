#include "MenuButton.h"

MenuButton::MenuButton() {
	position[0] = 0.0f;
	position[1] = 0.0f;
	size[0] = 0.0f;
	size[1] = 0.0f;
}

MenuButton::MenuButton(const char *fileName, float x, float y, float w, float h) {
	init(fileName, x, y, w, h);
}

void MenuButton::init(const char *fileName, float x, float y, float w, float h) {
	position[0] = x;
	position[1] = y;
	size[0] = w;
	size[1] = h;

	initBatch();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	loadTexture(fileName, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
}

MenuButton::~MenuButton() {
	unloadTexture();
}

void MenuButton::initBatch() {
	batch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	batch.MultiTexCoord2f(0, 0.0f, 0.0f);
	batch.Vertex3f(position[0], position[1], 0.0f);

	// Upper left hand corner
	batch.MultiTexCoord2f(0, 0.0f, 1.0f);
	batch.Vertex3f(position[0], position[1]+size[1], 0.0f);  

	// Upper right hand corner
	batch.MultiTexCoord2f(0, 1.0f, 1.0f);
	batch.Vertex3f(position[0]+size[0], position[1]+size[1], 0.0f);

	// Lower right hand corner
	batch.MultiTexCoord2f(0, 1.0f, 0.0f);
	batch.Vertex3f(position[0]+size[0], position[1], 0.0f);

	batch.End();
}

bool MenuButton::loadTexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
	GLbyte *pBits;
	int nWidth, nHeight, nComps;
	GLenum eFormat;

	// Read texture bits
	pBits = gltReadTGABits(fileName, &nWidth, &nHeight, &nComps, &eFormat);
	if (pBits == NULL)
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComps, nWidth, nHeight, 0,
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

void MenuButton::draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager) {
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, texture);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, pipeline.GetModelViewProjectionMatrix(), 0);
	batch.Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void MenuButton::unloadTexture() {
	glDeleteTextures(1, &texture);
}

bool MenuButton::isClicked(float x, float y) {
	return (x >= position[0] && x <= position[0]+size[0] &&
			y >= position[1] && y <= position[1]+size[1]);
}