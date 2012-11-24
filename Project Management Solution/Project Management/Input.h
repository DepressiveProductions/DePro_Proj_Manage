#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include <vector>
using std::vector;

class Input
{
public:
	Input();
	void passiveMouseFunc(int x, int y);
	void updatePosition(GLFrame &camFrame, GLMatrixStack &projStack);
	void mouseClick(int key, int state, int x, int y);
	
	bool keyPressed(int key);
	bool getClickState(); // Returns true if left-click is down
	void getMousePos2(M3DVector2f &pos);
	void getMousePos3(M3DVector3f &pos);
	~Input();

private:
	M3DVector3f mousePos3;
	float camSpeed;
	bool leftClick;
	int screenX;
	int screenY;
	int clickStartX;
	int clickStartY;
	int clickEndX;
	int clickEndY;
};