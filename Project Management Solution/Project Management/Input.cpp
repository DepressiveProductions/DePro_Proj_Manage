#include "Input.h"

Input::Input() {}

bool Input::keyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool Input::mouseButtonPressed(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}
/*
void Input::getMousePos2(sf::Vector2i &pos)
{
	pos = sf::Mouse::getPosition();
}
*/
/*
void Input::getMousePos3(sf::Vector3f &pos, GLFrame &camFrame, GLMatrixStack &projStack)
{
	sf::Vector2i cursorPos = sf::Mouse::getPosition();
	M3DMatrix44f mCamera;
	camFrame.GetCameraMatrix(mCamera);
	M3DMatrix44f mProjection;
	projStack.GetMatrix(mProjection);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	for (int i=0; i < 16; i++)
	{
		modelview[i] = mCamera[i];
		projection[i] = mProjection[i];
	}

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)cursorPos.x;
	winY = viewport[3] - (float)cursorPos.y;
	glReadPixels(cursorPos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	pos.x = posX;
	pos.y = posY;
	pos.z = posZ;
}
*/
Input::~Input(void)
{
}
