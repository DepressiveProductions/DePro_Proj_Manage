#include "Font.h"

Font::Font(void) {}
Font::~Font(void) {}

void Font::init(const char * imagepath)
{
	
}

void Font::showText(std::string text, float x, float y, float width, float height)
{
	
}

void Font::clearTexture()
{
	glDeleteTextures(1, uiTexture);
}