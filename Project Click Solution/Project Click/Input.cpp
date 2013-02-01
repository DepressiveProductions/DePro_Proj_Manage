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
array<float,2>	Input::clickPos;
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
		clickPos[0] = (float)x;
		clickPos[1] = (float)y;
	}
}