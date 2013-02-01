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

//////////////////////////////////////////////////////////////////////////////////////////

//GLTools objects:

GLGeometryTransform tPipeline;
GLShaderManager gltShaderManager;
GLMatrixStack projectionStack;
GLMatrixStack modelViewStack;
GLFrame cameraFrame;
GLFrustum viewFrustum;

//////////////////////////////////////////////////////////////////////////////////////////

//Objects:

Game gameLayer;
Shaders customShaders;
Background bg;
Block block;

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
const array<float,3>	blockPos	= {0.0f, 0.0f, 0.0f};
const float				blockWidth	= 1.0f;
const float				blockHeight = 1.0f;
const float				blockDepth	= 1.0f;

//////////////////////////////////////////////////////////////////////////////////////////

//Function primitives:

void setup();											//One-time setup function
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void handleInput();										//Handles keyboard input
void clickFunc(int key, int state, int x, int y);		//Handles mouse clicking

//////////////////////////////////////////////////////////////////////////////////////////

//Function definitions:

//One-time setup function:
void setup()
{
	//Set to play state:
	Globals::state = Globals::STATE_PLAY;

	//Background:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth testing and multisampling, set blending function:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	//Initialize GLTools' stock shaders:
	gltShaderManager.InitializeStockShaders();

	//Initialize custom shaders:
	customShaders.initADSVert();
	customShaders.initDiffVert();

	//Initiate game layer:
	gameLayer.init();

	//Set initial camera position:
	cameraFrame.MoveForward(bgZpos);
	cameraFrame.MoveUp(camYShift);
	cameraFrame.RotateWorld(camTilt, 1.0f, 0.0f, 0.0f);

	//Initiate background:
	bg.init(bgWidth, bgHeight, 0.0f);

	//More initiations below here ...
	block.init(blockPos[0], blockPos[1], blockPos[2], blockWidth, blockHeight, blockDepth);
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

	} else if (Globals::state ==Globals::STATE_PLAY) 
	{

		//Lighting variables:
		static M3DVector4f vLightPos = {1.5f, 0.0f, 0.0f, 1.0f};
		static M3DVector4f vAmbient = {0.1f, 0.1f, 0.1f, 1.0f};
		static M3DMatrix44f mCamera;

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
		block.draw(gltShaderManager, tPipeline, modelViewStack, vLightEyePos);

		//Draw background:
		bg.draw(gltShaderManager, tPipeline);
		
		//Camera matrix pop:
		modelViewStack.PopMatrix();
		
		//Ending pop:
		modelViewStack.PopMatrix();
		
		//Swap buffers and tell glut to keep looping:
		glutSwapBuffers();
		glutPostRedisplay();
		
		//Processor heavy:
		
		//Input:
		handleInput();
		
		//Game layer:
		//gameLayer.showInfo();
	}
}

//Handles keyboard input:
void handleInput() 
{

}

//Handles mouse clicking:
void clickFunc(int key, int state, int x, int y) 
{

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
	glutMouseFunc(clickFunc);

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
	system("PAUSE");
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////