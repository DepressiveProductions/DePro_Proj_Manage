#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <string>

#include "Shaders.h"

class MenuButton
{
public:
	MenuButton(); // If this is used, calling "init" will be necessary
	MenuButton(const char *fileName, float x, float y, float w, float h);
	~MenuButton();
	void init(const char *fileName, float x, float y, float w, float h);
	bool loadTexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void draw(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	void unloadTexture();

	bool isClicked(float x, float y);

private:
	float size[2];
	float position[2];
	GLuint texture;
	GLBatch batch;

	void initBatch();
};

