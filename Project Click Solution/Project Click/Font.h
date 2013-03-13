#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "stb_truetype.h"

using std::vector;
using std::map;

class Font
{
public:
	Font(void);
	~Font(void);

	void init();
	void showText(char *text, float x, float y, 
					float width, float height, GLShaderManager &sm,
					GLGeometryTransform tp);
	void draw(GLBatch &ba, GLShaderManager &sm, GLGeometryTransform tp);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	GLuint				uiTexture;
	GLfloat				vColor[4];
	stbtt_bakedchar		cdata[96]; // ASCII 32..126 is 95 glyphs
};