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
#include <string>
#include <map>
#include <StopWatch.h>
#include "Shaders.h"
#include "Background.h"
#include "Globals.h"
#include "Blocks.h"
#include "Input.h"
#include "UserInterface.h"
#include "Font.h"
#include "TexFont.h"
#include "font_parser.h"


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

Shaders					customShaders;
Blocks					blocks;
CStopWatch				gameTime;
Background				bg;
UserInterface			restartInfo;
UserInterface			alphaButton;
UserInterface			survivalButton;
UserInterface			optionsButton;
UserInterface			exitButton;
Font					font;


//////////////////////////////////////////////////////////////////////////////////////////

//Global vars:

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_TITLE "Project Click - Alpha"

float					finalTime;

const float				bgWidth		= 12.0f;
const float				bgHeight	= 5.0f;
const float				bgZpos		= -10.0f;

const float				mnuBtnLeft	= 25.0f;
const float				mnuBtnRight	= 75.0f;

const float				camTilt		= 0.15f;				//Looks nicer, stronger 3D effect
const float				camYShift	= -1.5f;				//To compensate for tilt

const array<float,3>	blockPos	= {0.0f, 0.0f, 0.0f};

M3DMatrix44f			mCamera;							//Handy to have it in global namespace
M3DMatrix44f			mOrtho;
M3DMatrix44f			mIdentity;

std::map< char, std::string > fontStrings;			//Map for all font texture paths


//////////////////////////////////////////////////////////////////////////////////////////

//Function primitives:

void setup();											//One-time setup function
void shutdownRC();										//One-time shutdown function
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void checkInput();										//Checks if any relevant input has been registred
void menuClick();
void menuKey();
void playClick();
void playKey();
void playRender();
void menuRender();

//////////////////////////////////////////////////////////////////////////////////////////

//Function definitions:

void
minifySelect(int value)
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value);
  glutPostRedisplay();
}

void
alphaSelect(int value)
{
  switch (value) {
  case GL_ALPHA_TEST:
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.5);
    break;
  case GL_BLEND:
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;
  case GL_NONE:
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    break;
  }
}

//One-time setup function:
void setup()
{
	fontStrings = getFontStrings("Assets/FONTS/ALPHA/");

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

	//Identity matrix, handy to have
	m3dLoadIdentity44(mIdentity);

	//Initialize custom shaders:
	customShaders.initADSVert();
	customShaders.initDiffVert();
	customShaders.initUI();

	//Initiate game layer:

	//Set initial camera position:
	cameraFrame.MoveForward(bgZpos);
	cameraFrame.MoveUp(camYShift);
	cameraFrame.RotateWorld(camTilt, 1.0f, 0.0f, 0.0f);

	//Initiate background:
	bg.init(bgWidth, bgHeight, 0.0f);

	//Initiate UI elements:
	restartInfo.init(17.0f, 43.0f, 83.0f, 53.0f, 0.5f, "Assets/FONT_BLOCK.tga");
	alphaButton.init(mnuBtnLeft, 55.0f, mnuBtnRight, 65.0f, 0.0f, "Assets/button_alpha.tga");
	survivalButton.init(mnuBtnLeft, 45.0f, mnuBtnRight, 55.0f, 0.0f, "Assets/button_survival.tga");
	optionsButton.init(mnuBtnLeft, 35.0f, mnuBtnRight, 45.0f, 0.0f, "Assets/button_options.tga");
	exitButton.init(mnuBtnLeft, 25.0f, mnuBtnRight, 35.0f, 0.0f, "Assets/button_quit.tga");
	//font.init(fontStrings);		//not vector anymore


	alphaSelect(GL_ALPHA_TEST);
	minifySelect(GL_NEAREST);


	//More initiations below here ...
	blocks.init(bgWidth, bgHeight, 0.0f);
}

void shutdownRC()
{
	glutDestroyWindow(glutGetWindow());

	alphaButton.clearTexture();
	survivalButton.clearTexture();
	optionsButton.clearTexture();
	exitButton.clearTexture();

	font.clearTexture();
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
	if (Globals::state == Globals::STATE_MENU) {
		menuRender();
	} else if (Globals::state ==Globals::STATE_PLAY) {
		playRender();
	}
	checkInput();
}

void playClick()
{
	//Ingame clicking
	M3DMatrix44f mProjection;
	projectionStack.GetMatrix(mProjection);
	array<float,3> clickPos = Input::checkClicked(Input::clickPos[0], Input::clickPos[1], mCamera, mProjection);
	
	if (Globals::nBlocks > 0 && blocks.remove(clickPos[0], clickPos[1], clickPos[2]))
	{
		Globals::nBlocks -= 1;

		if (Globals::nBlocks <= 0)
		{
			finalTime = gameTime.GetElapsedSeconds();	//Save final time to finalTime
			finalTime = floorf(finalTime * 100 + 0.5) / 100;  // Round to nearest 2nd decimal
			if (finalTime <= 2.50)
			{
				std::cout << "You did it in: " << finalTime <<" | Rating: GODLIKE!!!" << std::endl;	//Print final time GODLIKE
			}		
			else if (finalTime <= 3.00)
			{
				std::cout << "You did it in: " << finalTime <<" | Rating: MASTER!" << std::endl; //Print final time MASTER
			}
			else if (finalTime <= 4.5)
			{
				std::cout << "You did it in: " << finalTime <<" | Rating: REGULAR" << std::endl; //Print final time not bad
			}
			else
			{
				std::cout << "You did it in: " << finalTime <<" | Rating: Slow as a f****** granny on a highway!" << std::endl;	//Print final time slow as f
			}
		}

	}
}

void playKey()
{
	if (Globals::nBlocks <= 0 && Input::pressedKey == ' ')
	{
		blocks.sendWave(10);
		Globals::state = Globals::STATE_PLAY;
		gameTime.Reset();				  //Reset timer
	} else if (Input::pressedKey == 'o') {
		Globals::state = Globals::STATE_MENU;
		blocks.removeAll();
		Globals::nBlocks = 0;
	} else if (Input::pressedKey == 27) { //Escape
		shutdownRC();
		exit(0);
	}
}

void menuClick()
{
	//UI clicking:
	array<float,2> clickPos = Input::getUICoords(Input::clickPos[0], Input::clickPos[1]);

	if (clickPos[0] > mnuBtnLeft && clickPos[0] < mnuBtnRight && clickPos[1] < 65.0f) {
		if (clickPos[1] < 35.0f) {			//Quit
			shutdownRC();
			exit(0);
		} else if (clickPos[1] < 45.0f) {	//Options

		} else if (clickPos[1] < 55.0f) {	//Survival
			
		} else {							//Alpha
			Globals::state = Globals::STATE_PLAY;
			blocks.sendWave(10);
			gameTime.Reset();				//Reset timer

		}
	}
}

void menuKey()
{
	if (Input::pressedKey == ' ') {
		blocks.sendWave(10);
		Globals::state = Globals::STATE_PLAY;
	} else if (Input::pressedKey == 27) { //Escape
		shutdownRC();
		exit(0);
	}
}

void checkInput()
{
	if (Input::hasClicked) {
		Input::hasClicked = false;
		if (Globals::state == Globals::STATE_PLAY) {
			playClick(); 
		} else if (Globals::state == Globals::STATE_MENU) {
			menuClick();
		}
	}
	if (Input::hasPressed) {
		Input::hasPressed = false;
		if (Globals::state == Globals::STATE_PLAY) {
			playKey();
		} else if (Globals::state == Globals::STATE_MENU) {
			menuKey();
		}
	}
	if (Input::hasPressedSpecial) {
		Input::hasPressedSpecial = false;
	}
}

void playRender()
{
	//Lighting variables:
	static M3DVector4f vLightPos = {1.5f, 1.0f, 15.0f, 1.0f};
	static M3DVector4f vAmbient = {0.1f, 0.1f, 0.1f, 1.0f};
			
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
	blocks.draw(&customShaders, &tPipeline, &modelViewStack, vLightEyePos, vAmbient);

	//Draw background:
	bg.draw(gltShaderManager, tPipeline);
	
	//Camera matrix pop:
	modelViewStack.PopMatrix();
	
	//Ending pop:
	modelViewStack.PopMatrix();

	//UI:
	
	//Render a thing in the thing on the thing:
	if (Globals::nBlocks <= 0)
	{
		restartInfo.draw(uiPipeline, gltShaderManager);
	}

	//Swap buffers and tell glut to keep looping:
	glutSwapBuffers();
	glutPostRedisplay();
}

void menuRender()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	char *str;
	str = "DePro";

	alphaButton.draw(uiPipeline, gltShaderManager);
	survivalButton.draw(uiPipeline, gltShaderManager);
	optionsButton.draw(uiPipeline, gltShaderManager);
	exitButton.draw(uiPipeline, gltShaderManager);

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