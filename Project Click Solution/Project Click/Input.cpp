#include "Input.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


//Vars:

bool			Input::hasClicked = false;
bool			Input::hasPressedSpecial = false;
bool			Input::hasPressed = false;
array<int,2>	Input::clickPos;
unsigned char	Input::pressedKey;
int				Input::pressedSpecialKey;

//Constr and destr:

Input::Input(void) {}
Input::~Input(void) {}


//Methods:

void Input::specialKeyBoard(int key, int x, int y)
{
	switch (key)
	{
	default:
		break;
	}
}

void Input::keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //Escape
		glutDestroyWindow(glutGetWindow());
		exit(0);
		break;
	case ' ':
		hasPressed = true;
		pressedKey = key;
		break;
	default:
		break;
	}
}

void Input::mouseClick(int key, int state, int x, int y)
{
	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		hasClicked = true;
		clickPos[0] = x;
		clickPos[1] = y;
	}
}

array<float,3> Input::checkClicked(int x, int y, M3DMatrix44f mCamera, M3DMatrix44f mProj)
{
	//M3DVector2f cursor = {x, y};

	GLint viewport[4];
	GLdouble mv[16];
	GLdouble proj[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	for (int i=0; i < 16; i++)
	{
		mv[i] = mCamera[i];
		proj[i] = mProj[i];
	}

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = viewport[3] - (float)y;

	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, mv, proj, viewport, &posX, &posY, &posZ);

	array<float,3> retPos = {posX, posY, posZ};
	return retPos;
}