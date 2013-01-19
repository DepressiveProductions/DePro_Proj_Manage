#include "Button.h"


Button::Button(void)
{
}


Button::~Button(void)
{
}


void Button::init(float x, float y, float width, float height, float initialWindowWidth, float initialWindowHeight, std::string texFileName)
//void Button::init(int x, int y, int width, int height, std::string texFileName)
{
	//Set values, convert from pixels to float between -1 and 1
	xVal = ((x/initialWindowWidth)*2)-1;
	yVal = ((y/initialWindowHeight)*2)-1;
	widthVal = ((width/initialWindowWidth)*2)-1;
	heightVal = ((height/initialWindowHeight)*2)-1;		std::cout << xVal << std::endl << yVal << std::endl << widthVal << std::endl << heightVal << std::endl;
/*	xVal = x;
	yVal = y;
	widthVal = width;
	heightVal = height; */

	buttonBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Upper left hand corner
	buttonBatch.MultiTexCoord2f(0, 0.0f, height);
	buttonBatch.Vertex3f(x, y, 0.0);
        
	// Lower left hand corner
	buttonBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	buttonBatch.Vertex3f(x, y - height, 0.0f);

	// Lower right hand corner
	buttonBatch.MultiTexCoord2f(0, width, 0.0f);
	buttonBatch.Vertex3f(x + width, y - height, 0.0f);

	// Upper right hand corner
	buttonBatch.MultiTexCoord2f(0, width, height);
	buttonBatch.Vertex3f(x + width, y, 0.0f);

	buttonBatch.End();

	// Make texture object
	glGenTextures(1, &uiTexture);

	// Load the button
	glBindTexture(GL_TEXTURE_RECTANGLE, uiTexture);
	loadTGATextureRect(texFileName.c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);

	rectReplaceShader = gltLoadShaderPairWithAttributes("Shaders/RectReplace.vp", "Shaders/RectReplace.fp", 
			2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");
                 

	locRectMVP = glGetUniformLocation(rectReplaceShader, "mvpMatrix");
	locRectTexture = glGetUniformLocation(rectReplaceShader, "rectangleImage");
}


void Button::draw(GLfloat wWidth, GLfloat wHeight)
{
	// Creating this matrix really doesn't need to be done every frame. I'll leave it here
	// so all the pertenant code is together
	M3DMatrix44f mScreenSpace;
	m3dMakeOrthographicMatrix(mScreenSpace, 0.0f, wWidth, 0.0f, wHeight, -1.0f, 1.0f);
        
	// Turn blending on, and depth testing off
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(rectReplaceShader);
	glUniform1i(locRectTexture, 0);
	glUniformMatrix4fv(locRectMVP, 1, GL_FALSE, mScreenSpace);
	glBindTexture(GL_TEXTURE_RECTANGLE, uiTexture);
	buttonBatch.Draw();

	// Restore no blending and depth test
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}


// Do shutdown for the rendering context
void Button::clearTexture(void)
{
	glDeleteTextures(1, &uiTexture);
}


float Button::getXPos()
{
	return xVal;
}


float Button::getYPos()
{
	return yVal;
}


float Button::getWidth()
{
	return widthVal;
}


float Button::getHeight()
{
	return heightVal;
}


bool Button::loadTGATextureRect(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, magFilter);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);

	return true;
}