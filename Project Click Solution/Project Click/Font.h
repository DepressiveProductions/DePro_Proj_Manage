#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <string>
#include <iostream>
#include <vector>

using std::vector;

class Font
{
public:
	Font(void);
	~Font(void);

	void init(vector<std::string> texFileNames);
	void showText();
	void removeText();
	void initiateBatch(float x, float y, float width, float height = 0.0f);
	void drawAll(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	struct letter {
		GLBatch lBatch;
		int iTexture;
	};

	vector< letter > letters;

	GLuint		uiTexture;
	int			nTextures;
	float		xmin, ymin, xmax, ymax, z;
};

