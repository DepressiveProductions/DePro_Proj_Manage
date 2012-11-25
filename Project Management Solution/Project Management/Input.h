#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

class Input
{
public:
	Input();
	bool keyPressed(sf::Keyboard::Key key);
	bool mouseButtonPressed(sf::Mouse::Button button); // Returns true if left-click is down
	void getMousePos2(sf::Vector2i &pos);
	void getMousePos3(sf::Vector3f &pos, GLFrame &camFrame, GLMatrixStack &projStack);
	~Input();

private:
	float camSpeed;
	bool leftClick;
	int clickStartX;
	int clickStartY;
	int clickEndX;
	int clickEndY;
};