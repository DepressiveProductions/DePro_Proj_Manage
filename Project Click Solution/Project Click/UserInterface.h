#pragma once

#include <GLTools.h>
#include <GLShaderManager.h>
#include <string>

#include "Shaders.h"

class UserInterface
{
public:
	UserInterface(void);
	~UserInterface(void);

	void init(float x, float y, float z, float width, float height, std::string texFileName);
	void draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	bool loadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	void initiateBatch();

	GLuint		uiTexture;
	GLBatch		rectBatch;
	GLFrame		uiFrame;
	float		x, y, z, w, h;

};

