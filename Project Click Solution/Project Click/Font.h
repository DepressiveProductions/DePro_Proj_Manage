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

	struct letter {
		GLBatch	lBatch;
		int		iTexture;
		char	c;
	};

	void init(map<char,std::string> texFileNames);
	void showText(std::string text, float x, float y, float width, float height = 0.0f);
	void removeText(std::string key);
	void initiateBatch(letter *l, float x, float y, float width, float height);
	void drawAll(GLGeometryTransform pipeline, GLShaderManager &shaderManager);
	bool loadTGATexture(const char *szFileNames, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
	void clearTexture(void); // Goes in main before terminating program

private:
	map< std::string, vector< letter * >> allText;
	map< char, int > texIDs;

	GLuint		uiTexture[50];
	int			nTextures;
	float		xmin, ymin, xmax, ymax, z;
};