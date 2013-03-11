#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;

class Font
{
public:
	Font(void);
	~Font(void);

	void init();
	void showText(std::string text, float x, float y, float width, float height = 0.0f);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	GLuint		*uiTexture;
	float		xmin, ymin, xmax, ymax, z;
};