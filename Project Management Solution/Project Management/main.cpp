//STATES:
//#define HOUSE_DEBUG
//#define TRIANGLE_DEBUG
//#define DEBUG
#define PATH_DEBUG

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
#include <vector>
#include <thread>
#include <array>

#include "House.h"
#include "Path.h"
#include "Button.h"
#include "Floor.h"
#include "Grid.h"
#include "Input.h"
#include "MyShaderManager.h"
#include "Character.h"

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

float camSpeed = 5.5f;
array<float, 3> clickPos;
array<float, 3> actualPos;
array<float, 3> lastPos;
float mouseLook = 50.0f;
bool buildMode = false;
bool trackCursor = false;
bool mouseActive = false;

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
Path pf;
Floor ground;
Grid hlGrid; // Highlight grid
Button buildButton;
Character buildMan;

#ifdef TRIANGLE_DEBUG
GLBatch testBatch;
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
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	//Initialize stock shaders from GLTools:
	shaderManager.InitializeStockShaders();

	//Experimental first self-written shaders:
	emilShaders.initDiffVert();
	emilShaders.initADSVert();
	//emilShaders.initADSFrag();
	
	//Move cam back:
	cameraFrame.MoveForward(-20.0f);

	//Init debug batches:
	#ifdef TRIANGLE_DEBUG
	testBatch.Begin(GL_TRIANGLES, 3);
		testBatch.Vertex3f(1.0f, -1.0f, 0.0f);
		testBatch.Vertex3f(0.0f, 1.0f, 0.0f);
		testBatch.Vertex3f(-1.0f, -1.0f, 0.0f);
	testBatch.End();
	#endif

	//buildButton.init((glutGet(GLUT_WINDOW_WIDTH)/40), (glutGet(GLUT_WINDOW_HEIGHT)/30)+32, 128, 32, W_WIDTH, W_HEIGHT, "Assets/button_build_128x32.tga");
	buildButton.init((glutGet(GLUT_WINDOW_WIDTH)/40), (glutGet(GLUT_WINDOW_HEIGHT)/30)+32, 128, 32, "Assets/button_build_128x32.tga");

	ground.init(0.0f, 0.0f, 0.0f, 20, 20, C_RAD);

	hlGrid.init(0.0f, 0.0f, 0.0f, C_RAD);

	M3DVector4f baracksShine = {0.5, 0.5, 0.5, 1.0};
	M3DVector4f baracksColor = {0.3f, 0.3f, 0.3f, 1.0f};
	baracks.init(C_RAD, baracksShine, baracksColor);
	pf.addBuildingType(&baracks);

	array<float, 3> spawnPos = {0.0f, 0.0f, 0.2f};
	buildMan.init(0.2, 0.2f, spawnPos);
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
	//if(!mouseActive)
	//	modelViewStack.Rotate(-15.0f, 1.0f, 0.0f, 0.0f);

	//Light source:
	static M3DVector4f vAmbient = {0.1f, 0.1f, 0.1f, 1.0f};
	static M3DVector4f vLightPos = {0.0f, 0.0f, 4.0f, 1.0f};
	static M3DVector4f vLightEyePos;
	m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

	// Floor
	ground.draw(shaderManager, tPipeline, modelViewStack);

	// Highlight grid
	hlGrid.draw(shaderManager, tPipeline, modelViewStack);
	
	// Draw paths
#ifdef PATH_DEBUG
	pf.draw(shaderManager, tPipeline, modelViewStack);
#endif

	#ifdef TRIANGLE_DEBUG
	//Draw debug batches:
	modelViewStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vDarkRed);
	testBatch.Draw();
	modelViewStack.PopMatrix();
	#endif

	// Baracks
	baracks.drawAll(emilShaders, tPipeline, vLightEyePos, modelViewStack, vAmbient);

	buildMan.draw(shaderManager, tPipeline, modelViewStack, vLightEyePos);

	//End cam push:
	modelViewStack.PopMatrix();

	//End pop:
	modelViewStack.PopMatrix();
	
	buildButton.draw(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glutSwapBuffers();
	glutPostRedisplay();

	// Input
	handleInput(inputTimer);
	buildMan.move();
}

void handleInput(CStopWatch &inputTimer)
{	
	// Exit
	if (in.keyPressed(sf::Keyboard::Escape))
		exit(0);

	// Camera movement
	float elapsedTime = inputTimer.GetElapsedSeconds() * 1.5f;
	inputTimer.Reset();

	if (in.keyPressed(sf::Keyboard::B))
	{
		buildMode = !buildMode;
		hlGrid.deactivateAllSquares();
		ground.toggleGrid();
	}

	if (mouseActive)
	{
		if (in.keyPressed(sf::Keyboard::W))
			cameraFrame.MoveForward(camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::S))
			cameraFrame.MoveForward(-camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::A))
			cameraFrame.MoveRight(camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::D))
			cameraFrame.MoveRight(-camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::Space))
			cameraFrame.MoveUp(camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::LControl))
			cameraFrame.MoveUp(-camSpeed*elapsedTime);
	}

	else
	{
		if (in.keyPressed(sf::Keyboard::W))
			cameraFrame.MoveUp(camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::S))
			cameraFrame.MoveUp(-camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::A))
			cameraFrame.MoveRight(camSpeed*elapsedTime);
		if (in.keyPressed(sf::Keyboard::D))
			cameraFrame.MoveRight(-camSpeed*elapsedTime);
	}
}

void releasedKeys(unsigned char key, int x, int y)
{
	if (key == VK_MENU)
	{
		if (mouseActive)
		{
			mouseActive = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
		else if (!mouseActive)
		{
			glutWarpPointer(W_WIDTH/2, W_HEIGHT/2);
			mouseActive = true;
			glutSetCursor(GLUT_CURSOR_NONE);
		}
	}
}

void clickFunc(int key, int state, int x, int y)
{
	int y2d = glutGet(GLUT_WINDOW_HEIGHT)-((W_HEIGHT+y)-glutGet(GLUT_WINDOW_HEIGHT));
	
	//If LMB is clicked:
	if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		//If mouse on build button:	
		if (x >= buildButton.getXPos() && x <= buildButton.getXPos() + buildButton.getWidth() && y2d >= buildButton.getYPos()-buildButton.getHeight() && y2d <= buildButton.getYPos())
		{
			buildMode = !buildMode;
			hlGrid.deactivateAllSquares();
			ground.toggleGrid();
		}

		else
		{
			in.getCursor3(x, y, clickPos, cameraFrame, projectionStack);
			if (buildMode)
			{
				hlGrid.deactivateAllSquares();
			}
			else if (!buildMode)
			{
				trackCursor = true;
				float xx = float(x)/mouseLook;
				float yy = float(y)/mouseLook;
				lastPos[0] = xx;
				lastPos[1] = yy;
			}
		}
	}
	
	//If LMB is released:
	else if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		//If mouse not on button:
		if (!(x >= buildButton.getXPos() && x <= buildButton.getXPos() + buildButton.getWidth() && y2d >= buildButton.getYPos()-buildButton.getHeight() && y2d <= buildButton.getYPos()) && buildMode)
		{
			in.getCursor3(x, y, actualPos, cameraFrame, projectionStack);
			hlGrid.boxActivation(clickPos, actualPos);
			
			vector<array<float, 3>> pos;
			hlGrid.getSquarePositions(pos);
			baracks.create(pos);
		}
		//Else if 
		else if ((x >= buildButton.getXPos() && x <= buildButton.getXPos() + buildButton.getWidth() && y2d >= buildButton.getYPos()-buildButton.getHeight() && y2d <= buildButton.getYPos()) && buildMode)
		{
			hlGrid.deactivateAllSquares();
		}
		else if (!buildMode)
			trackCursor = false;
	}

	//If RMB is clicked:
	if ((key == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		//If mouse not on button:
		if (!(x >= buildButton.getXPos() && x < buildButton.getXPos()+buildButton.getWidth() && y2d <= buildButton.getYPos() && y2d >= buildButton.getYPos()-buildButton.getHeight()))
		{
			if (!buildMode)
			{
				in.getCursor3(x, y, actualPos, cameraFrame, projectionStack);
				buildMan.moveTo(actualPos);
			}
			//For canceling the boxing of the ground and the construction:
			else if (buildMode)
			{
				hlGrid.deactivateAllSquares();
			}
		}
	}

	//Zoom controls:
	if (!mouseActive)
	{
		if (key == 3) //Scroll up
			cameraFrame.TranslateWorld(0.0f, 0.0f, -0.5f); //Zoom in
		if (key == 4) //Scroll down
			cameraFrame.TranslateWorld(0.0f, 0.0f, 0.5f); //Zoom out
	}
}


void mousePassiveFunc(int x, int y)
{
		if (buildMode)
		{
			in.getCursor3(x, y, actualPos, cameraFrame, projectionStack);
			hlGrid.deactivateAllSquares();
			hlGrid.boxActivation(clickPos, actualPos);
		}
		else if (!buildMode && trackCursor)
		{
			float xx = float(x)/mouseLook;
			float yy = float(y)/mouseLook;
			cameraFrame.MoveRight(xx-lastPos[0]);
			cameraFrame.MoveUp(yy-lastPos[1]);
			lastPos[0] = xx;
			lastPos[1] = yy;
		}
}

void mouseCameraFunc(int x, int y)
{
	if (mouseActive)
	{
		GLfloat angle = 0.005f;

		if (x > W_WIDTH/2)
		{
			cameraFrame.RotateWorld(-angle*(x-W_WIDTH/2), 0, 0, 1);
			glutWarpPointer(W_WIDTH/2, W_HEIGHT/2);
		}
		else if (x < W_WIDTH/2)
		{
			cameraFrame.RotateWorld(angle*(W_WIDTH/2-x), 0, 0, 1);
			glutWarpPointer(W_WIDTH/2, W_HEIGHT/2);
		}
		if (y > W_HEIGHT/2)
		{
			cameraFrame.RotateLocalX(-angle*(W_HEIGHT/2-y));
			glutWarpPointer(W_WIDTH/2, W_HEIGHT/2);
		}
		else if (y < W_HEIGHT/2)
		{
			cameraFrame.RotateLocalX(angle*(y-W_HEIGHT/2));
			glutWarpPointer(W_WIDTH/2, W_HEIGHT/2);
		}
	}
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//Initiate glut and create a window:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition( (glutGet(GLUT_SCREEN_WIDTH)-W_WIDTH)/2, // Places the window in the middle of the screen
							(glutGet(GLUT_SCREEN_HEIGHT)-W_HEIGHT)/2);
	glutCreateWindow(W_TITLE);

	//Send funcs to gluttis:
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutKeyboardUpFunc(releasedKeys);
	glutPassiveMotionFunc(mouseCameraFunc);
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