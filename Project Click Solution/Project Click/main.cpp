//OpenGL Includes:

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLMatrixStack.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

//////////////////////////////////////////////////////////////////////////////////////////

//Other includes:

#include <math.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include "Game.h"
#include "Shaders.h"
#include "Background.h"
#include "Globals.h"
#include "Block.h"
#include "Input.h"
#include "UserInterface.h"

//////////////////////////////////////////////////////////////////////////////////////////

//GLTools objects:

GLGeometryTransform		tPipeline;
GLGeometryTransform		uiPipeline;
GLShaderManager			gltShaderManager;
GLMatrixStack			projectionStack;
GLMatrixStack			modelViewStack;
GLMatrixStack			uiProjStack;
GLMatrixStack			uiMVStack;
GLFrame					cameraFrame;
GLFrustum				viewFrustum;

//////////////////////////////////////////////////////////////////////////////////////////

//Objects:

Game					gameLayer;
Shaders					customShaders;
Background				bg;
Block					block;
UserInterface			playInfo;

//////////////////////////////////////////////////////////////////////////////////////////

//Global vars:

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_TITLE "Project Click - Alpha"

const float				bgWidth		= 40.0f;
const float				bgHeight	= 5.0f;
const float				bgZpos		= -10.0f;

const float				camTilt		= 0.15f;				//Looks nicer, stronger 3D effect
const float				camYShift	= -1.5f;				//To compensate for tilt

const float				blockWidth	= 1.0f;
const float				blockHeight = 1.0f;
const array<float,3>	blockPos	= {0.0f, 0.0f, 0.0f};

M3DMatrix44f			mCamera;							//Handy to have it in global namespace
M3DMatrix44f			mOrtho;



//////////////////////////////////////////////////////////////////////////////////////////

//Function primitives:

void setup();											//One-time setup function
void shutdownRC();										//One-time shutdown function
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void checkInput();										//Checks if any relevant input has been registred
void playRender();
void menuRender();

//////////////////////////////////////////////////////////////////////////////////////////

//Function definitions:

//One-time setup function:
void setup()
{
	//Set to play state:
	//Globals::state = Globals::STATE_PLAY;

	//Background:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth testing and multisampling, set blending function:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	//Initialize GLTools' stock shaders:
	gltShaderManager.InitializeStockShaders();

	//Orthographic transform pipeline for UI:
	viewFrustum.SetOrthographic(0.0f, 100.0f, 0.0f, 100.0f, -1.0f, 1.0f);
	uiProjStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	uiPipeline.SetMatrixStacks(uiMVStack, uiProjStack);

	//Initialize custom shaders:
	customShaders.initADSVert();
	customShaders.initDiffVert();
	customShaders.initUI();

	//Initiate game layer:
	gameLayer.init();

	//Set initial camera position:
	cameraFrame.MoveForward(bgZpos);
	cameraFrame.MoveUp(camYShift);
	cameraFrame.RotateWorld(camTilt, 1.0f, 0.0f, 0.0f);

	//Initiate background:
	bg.init(bgWidth, bgHeight, 0.0f);

	//Initiate UI elements:
	playInfo.init(40.0f, 40.0f, 0.0f, 20.0f, 20.0f, "Assets/button_build_128x32.tga");

	//More initiations below here ...

}

void shutdownRC()
{
	glutDestroyWindow(glutGetWindow());
	playInfo.clearTexture();
}

//Runs everytime the window 'changes size', for example when the window is created:
void changeSize(int w, int h)
{
	//Tell OpenGL about the new window size:
	glViewport(0, 0, w, h);

	//Don't divide by zero:
	if (h == 0) { h = 1; }

	//Tell GLTools about the new window size:
	viewFrustum.SetPerspective(35.0f, float(w)/float(h), 0.01f, 100.0f);
	projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
	tPipeline.SetMatrixStacks(modelViewStack, projectionStack);
}

//Basic glutfunc for rendering stuff. Runs every frame:
void renderScene()
{
	if (Globals::state == Globals::STATE_MENU)
	{
		menuRender();

	} else if (Globals::state ==Globals::STATE_PLAY) 
	{
		playRender();

	}
	checkInput();

}

void checkInput()
{
	if (Input::hasClicked)
	{
		Input::hasClicked = false;

		M3DMatrix44f mProjection;
		projectionStack.GetMatrix(mProjection);

		array<float,3> clickPos = Input::checkClicked(Input::clickPos[0], Input::clickPos[1], mCamera, mProjection);
	}
	if (Input::hasPressed)
	{
		Input::hasPressed = false;

		if (/*(*/Globals::state == Globals::STATE_MENU /*|| noBlocks)*/ && Input::pressedKey == ' ')
		{
			//restart();
			Globals::state = Globals::STATE_PLAY;
		}

		if (Input::pressedKey == 'o')
		{
			Globals::state = Globals::STATE_MENU;
		}

		if (Input::pressedKey == 27) //Escape
		{
			shutdownRC();
			exit(0);
		}
	}
	if (Input::hasPressedSpecial)
	{
		Input::hasPressedSpecial = false;
	}
}

void playRender()
{
	//Lighting variables:
	static M3DVector4f vLightPos = {1.5f, 1.0f, 11.0f, 1.0f};
	static M3DVector4f vAmbient = {0.2f, 0.2f, 0.2f, 1.0f};
			
	//Clear buffers:
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			
	//Beginning push:
	modelViewStack.PushMatrix();
	
	//Camera matrix:
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewStack.PushMatrix(mCamera);
	
	//Calc light pos in eye coords:
	static M3DVector4f vLightEyePos;
	m3dTransformVector4(vLightEyePos, vLightPos, mCamera);
	
	//Render stuff here:
	//TESTBLOCK:
	block.draw(customShaders, tPipeline, modelViewStack, vLightEyePos, vAmbient);

	//Draw background:
	bg.draw(gltShaderManager, tPipeline);
	
	//Camera matrix pop:
	modelViewStack.PopMatrix();
	
	//Ending pop:
	modelViewStack.PopMatrix();
	
	//Swap buffers and tell glut to keep looping:
	glutSwapBuffers();
	glutPostRedisplay();
}

void menuRender()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	playInfo.draw(uiPipeline, gltShaderManager);

	glutSwapBuffers();
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////////////////////

//Main:

int main(int argc, char* argv[]) //Starting point of the application
{
	//Initiate GLTools:
	gltSetWorkingDirectory(argv[0]);

	//Initiate glut and create a window:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-W_WIDTH)/2,
							(glutGet(GLUT_SCREEN_HEIGHT)-W_HEIGHT)/2); //Center of screen
	glutCreateWindow(W_TITLE);

	//Send funcs to glut:
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutMouseFunc(Input::mouseClick);
	//glutSpecialFunc(Input::specialKeyBoard);
	glutKeyboardFunc(Input::keyBoard);

	//Initiate GLEW, quit if it doesn't work:
	GLenum result = glewInit();
	if(result != GLEW_OK)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(result));
		return 1;
	}

	//Run setup and start looping:
	setup();
	glutMainLoop();
	shutdownRC();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////