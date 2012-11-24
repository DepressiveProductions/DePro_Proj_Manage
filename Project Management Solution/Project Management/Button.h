#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <GLBatch.h>

#include <string>

class Button
{
public:
	Button(void);
	~Button(void);

	void init(int x, int y, int width, int height, std::string texFileName); //Goes in SetupRC
	void draw(GLfloat wWidth, GLfloat wHeight); //Goes in renderScene
	void clearTexture(void); // Goes in main before terminating program
	/*
	int getXPos();
	int getYPos();
	int getWidth();
	int getHeight();
	*/
	bool loadTGATextureRect(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

private:
	GLBatch		buttonBatch;

	GLuint		uiTexture;
	GLint		rectReplaceShader;
	GLint		locRectMVP;
	GLint		locRectTexture;
/*
	int xVal;
	int yVal;
	int widthVal;
	int heightVal;*/
};

