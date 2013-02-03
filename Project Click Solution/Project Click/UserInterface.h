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

	void init(float xmin, float ymin, float xmax, float ymax, float z, std::string texFileName);
	void draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	bool loadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	void initiateBatch();

	GLuint		uiTexture;
	GLBatch		rectBatch;
	GLFrame		uiFrame;
	float		xmin, ymin, xmax, ymax, z;

};

