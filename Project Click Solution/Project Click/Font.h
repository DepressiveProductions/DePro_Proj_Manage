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

	void init(float xmin, float ymin, float xmax, float ymax, float z, vector<std::string> texFileNames);
	void draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	void initiateBatch();

	GLuint		uiTexture;
	int			nTextures;
	GLBatch		rectBatch;
	GLFrame		uiFrame;
	float		xmin, ymin, xmax, ymax, z;
};

