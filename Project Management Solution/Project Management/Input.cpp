//Includes:
#include "Input.h"

//Constructor:
Input::Input(void) 
{
}

//Destructor:
Input::~Input(void)
{
}

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

void Input::getCursor3(int x, int y, M3DVector3f &pos, GLFrame &cameraFrame, GLMatrixStack &projectionStack)
{
	M3DVector2f cursor = {x, y};
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	M3DMatrix44f mProjection;
	projectionStack.GetMatrix(mProjection);

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

	winX = (float)cursor[0];
	winY = viewport[3] - (float)cursor[1];
	glReadPixels(cursor[0], int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	pos[0] = posX;
	pos[1] = posY;
	pos[2] = posZ;
}


