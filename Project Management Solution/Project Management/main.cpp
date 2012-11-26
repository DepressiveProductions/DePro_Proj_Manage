//STATES:
//#define HOUSE_DEBUG
//#define TRIANGLE_DEBUG
//#define DEBUG

//Includes:
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <GLFrame.h>
#include <GLBatch.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <math.h>
#include <ctime>
#include <StopWatch.h>
#include <iostream>

#include "House.h"
#include "Button.h"
#include "Floor.h"
#include "Grid.h"
#include "Input.h"
#include "MyShaderManager.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define C_RAD 0.5f
#define W_WIDTH 800
#define W_HEIGHT 600
#define W_TITLE "Project: Management - Prototype"

float camSpeed = 4.0f;
M3DVector3f clickPos;
M3DVector3f actualPos;
bool buttonWasDown = false; // Left mouse button - used for not resetting clickPos every frame that the button is pressed

//Some important objects:
MyShaderManager emilShaders;
GLShaderManager shaderManager;
GLGeometryTransform tPipeline;
GLMatrixStack projectionStack;
GLMatrixStack modelViewStack;
GLFrustum viewFrustum;
GLFrame cameraFrame;
Input in;

House baracks;
Floor ground;
Grid hlGrid; // Highlight grid
Button buildButton;


#ifdef TRIANGLE_DEBUG
GLBatch testBatch;
#endif

#ifdef HOUSE_DEBUG
House testHouse;
#endif

void setupRC();								//One-time setup function (RC = Rendering Context)
void changeSize(int w, int h);				//Runs everytime the window 'changes size', for example when the window is created
void renderScene();							//Basic glutfunc for rendering stuff. Runs every frame
void handleInput(CStopWatch &inputTimer);	//Handles input ...

void setupRC()
{
	//BG:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth testing so things won't look effed up:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	//Initialize stock shaders from GLTools:
	shaderManager.InitializeStockShaders();

	//Experimental first self-written shader:
	emilShaders.initDiffVert();
	emilShaders.initADSVert();
	
	//Move cam back:
	cameraFrame.MoveForward(-10.0f);

	#ifdef HOUSE_DEBUG
	M3DVector3f pos3 = {-11*C_RAD, 11*C_RAD, 0.0f};
	M3DVector3f pos4 = {C_RAD, C_RAD, 0.0f};
	M3DVector3f pos = {C_RAD, C_RAD, 0.0f};
	M3DVector3f pos2 = {11*C_RAD, -11*C_RAD, 0.0f};
	M3DVector4f debugShine = {0.5, 0.5, 0.5, 1.0};
	M3DVector4f debugColor = {0.3f, 0.3f, 0.3f, 1.0f};
	testHouse.init(C_RAD, debugShine, debugColor);
	testHouse.create(pos2, pos, 12);
	testHouse.create(pos3, pos4, 14);
	#endif

	//Init debug batches:
	#ifdef TRIANGLE_DEBUG
	testBatch.Begin(GL_TRIANGLES, 3);
		testBatch.Vertex3f(1.0f, -1.0f, 0.0f);
		testBatch.Vertex3f(0.0f, 1.0f, 0.0f);
		testBatch.Vertex3f(-1.0f, -1.0f, 0.0f);
	testBatch.End();
	#endif

	buildButton.init(20, 50, 128, 32, "Assets/button_build_128x32.tga");

	ground.init(0.0f, 0.0f, 0.0f, 20, 20, C_RAD);

	hlGrid.init(0.0f, 0.0f, 0.0f, C_RAD);
	
	//Baracks building type:
	M3DVector4f baracksShine = {0.5, 0.5, 0.5, 1.0};
	M3DVector4f baracksColor = {0.3f, 0.3f, 0.3f, 1.0f};
	baracks.init(C_RAD, baracksShine, baracksColor);
}

void changeSize(int w, int h)
{
	glViewport(0, 0, w, h);

	if(h <= 0) {h = 1;}

	viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
	projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	tPipeline.SetMatrixStacks(modelViewStack, projectionStack);
}

void renderScene()
{
	//Colours:
	static GLfloat vDarkRed[] = {0.5f, 0.1f, 0.1f, 1.0f};

	static CStopWatch inputTimer;

	//Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	//Beginning push:
	modelViewStack.PushMatrix();

	//Camera matrix:
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewStack.PushMatrix(mCamera);

	//Light source:
	static M3DVector4f vAmbient = {0.1f, 0.1f, 0.1f, 1.0f};
	static M3DVector4f vLightPos = {0.0f, 0.0f, 4.0f, 1.0f};
	static M3DVector4f vLightEyePos;
	m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

	// Floor
	ground.draw(shaderManager, tPipeline, modelViewStack);

	// Highlight grid
	hlGrid.draw(shaderManager, tPipeline, modelViewStack);

	//Baracks:
	baracks.drawAll(emilShaders, tPipeline, vLightEyePos, modelViewStack, vAmbient);
		
	#ifdef TRIANGLE_DEBUG
	//Draw debug batches:
	modelViewStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vDarkRed);
	testBatch.Draw();
	modelViewStack.PopMatrix();
	#endif

	//House debug drawing:
	#ifdef HOUSE_DEBUG
	testHouse.drawAll(emilShaders, tPipeline, vLightEyePos, modelViewStack, vAmbient);
	#endif

	//End cam push:
	modelViewStack.PopMatrix();

	//End pop:
	modelViewStack.PopMatrix();
	
	buildButton.draw(W_WIDTH, W_HEIGHT);

	glutSwapBuffers();
	glutPostRedisplay();

	// Input
	handleInput(inputTimer);
}

void handleInput(CStopWatch &inputTimer)
{	
	// Exit
	if (in.keyPressed(sf::Keyboard::Escape))
		exit(0);

	// Camera movement
	float elapsedTime = inputTimer.GetElapsedSeconds() * 1.5f;
	inputTimer.Reset();
	
	if (in.keyPressed(sf::Keyboard::W))
		cameraFrame.MoveUp(camSpeed*elapsedTime);
	if (in.keyPressed(sf::Keyboard::S))
		cameraFrame.MoveUp(-camSpeed*elapsedTime);
	if (in.keyPressed(sf::Keyboard::A))
		cameraFrame.MoveRight(camSpeed*elapsedTime);
	if (in.keyPressed(sf::Keyboard::D))
		cameraFrame.MoveRight(-camSpeed*elapsedTime);

	// Mouse-clicks
	/*if (in.mouseButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i pos;
		in.getMousePos2(pos);
		
		// Check if cursor is over the build-button
		if ((pos.x >= buildButton.getXPos()) && (pos.x <= buildButton.getXPos() + buildButton.getWidth()) && (pos.y >= buildButton.getYPos()) && (pos.y <= buildButton.getYPos() + buildButton.getHeight()))
		{
			ground.addHLSquare(0.25f, 0.25f);
		}
	}*/
}

void getCursor3(int x, int y, M3DVector3f &pos)
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

void clickFunc(int key, int state, int x, int y)
{
	if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		// Click build-button
		if ((x >= buildButton.getXPos()) && (x <= buildButton.getXPos() + buildButton.getWidth()) && (W_HEIGHT - y >= buildButton.getYPos()-buildButton.getHeight()/2) && (W_HEIGHT - y <= buildButton.getYPos()+buildButton.getHeight()/2))
			ground.toggleGrid();
		/*else if (buttonWasDown)
		{
			getCursor3(x, y, actualPos);
			hlGrid.boxActivation(clickPos, actualPos);
		}*/
		else if (!buttonWasDown) // Runs first time
		{
			hlGrid.deactivateAllSquares();
			getCursor3(x, y, clickPos);
			buttonWasDown = true;
		}
	}
	else if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		// Do not act as ground if clicked on button
		if (!((x >= buildButton.getXPos()) && (x <= buildButton.getXPos() + buildButton.getWidth()) && (W_HEIGHT - y >= buildButton.getYPos()-buildButton.getHeight()/2) && (W_HEIGHT - y <= buildButton.getYPos()+buildButton.getHeight()/2)))
		{
			getCursor3(x, y, actualPos);
			hlGrid.boxActivation(clickPos, actualPos);
			buttonWasDown = false;
		}
	}
}

void mousePassiveFunc(int x, int y)
{
		if (buttonWasDown)
		{
			hlGrid.deactivateAllSquares();
			getCursor3(x, y, actualPos);
			hlGrid.boxActivation(clickPos, actualPos);
		}
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//Initiate glut and create a window:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition( (glutGet(GLUT_SCREEN_WIDTH)-W_WIDTH)/2, // Places the window in the middle of the screen
							(glutGet(GLUT_SCREEN_HEIGHT)-W_HEIGHT)/2);
	glutCreateWindow(W_TITLE);

	//Send funcs to gluttis:
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutMouseFunc(clickFunc);
	glutMotionFunc(mousePassiveFunc);

	GLenum result = glewInit();
	if(result != GLEW_OK)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(result));
		return 1;
	}
	
	//Setup stuff and start looping:
	setupRC();
	glutMainLoop();
	buildButton.clearTexture();
	system("PAUSE");
	return 0;
}