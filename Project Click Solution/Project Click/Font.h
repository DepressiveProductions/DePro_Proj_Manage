#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <string>
#include <iostream>

class Font
{
public:
	Font(void);
	~Font(void);

	void init(std::string fontFileName);
	void showText(char *text, float x, float y, 
					float width, float height, GLShaderManager &sm,
					GLGeometryTransform tp);
	void draw(GLBatch &ba, GLShaderManager &sm, GLGeometryTransform tp);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	GLuint				uiTexture;
	GLfloat				vColor[4];
};