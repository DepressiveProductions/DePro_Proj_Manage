#include "Input.h"

Input::Input() {}

void Input::passiveMouseFunc(int x, int y)
{
	screenX = x;
	screenY = y;
}

void Input::updatePosition(GLFrame &camFrame, GLMatrixStack &projStack)
{
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

	winX = (float)screenX;
	winY = viewport[3] - (float)screenY;
	glReadPixels(screenX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	mousePos3[0] = posX;
	mousePos3[1] = posY;
	mousePos3[2] = posZ;
}

void Input::mouseClick(int key, int state, int x, int y)
{
	screenX = x;
	screenY = y;
	if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_KEY_DOWN))
	{
		clickStartX = x;
		clickStartY = y;
		leftClick = true;
	}
	else if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_KEY_UP))
	{
		clickEndX = x;
		clickEndY = y;
		leftClick = false;
	}
}

bool Input::keyPressed(int key)
{
	if (GetKeyState(key) < 0)
		return true;
	else
		return false;
}

bool Input::getClickState()
{
	return leftClick;
}

void Input::getMousePos2(M3DVector2f &pos)
{
	pos[0] = screenX;
	pos[1] = screenY;
}

void Input::getMousePos3(M3DVector3f &pos)
{
	pos[0] = mousePos3[0];
	pos[1] = mousePos3[1];
	pos[2] = mousePos3[2];
}

Input::~Input(void)
{
}
