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
#include <ctime>
#include "Shaders.h"
#include "Background.h"
#include "Globals.h"
#include "Blocks.h"
#include "Input.h"
#include "UserInterface.h"
#include "Font.h"
#include "Sound.h"
#include "Timer.h"

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
Timer					gameTime;
Background				bg;
UserInterface			restartInfo;
UserInterface			alphaButton;
UserInterface			survivalButton;
UserInterface			optionsButton;
UserInterface			exitButton;
UserInterface			menuBG;
UserInterface			optSoundToggle;
UserInterface			optBackButton;
Font					font;
Sound					music;
Sound					menuClickSound;

//////////////////////////////////////////////////////////////////////////////////////////

//Global vars:

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_TITLE "Project Click - Alpha"

float					finalTime;

const float				bgWidth		= 12.0f;
const float				bgHeight	= 5.0f;
const float				bgZpos		= -10.0f;

const float				nMnuBtns		= 4.0f;
const float				mnuBtnLeft		= 25.0f;
const float				mnuBtnRight		= 75.0f;
const float				mnuBtnTop		= 65.0f;
const float				mnuBtnBot		= 25.0f;
const float				mnuBtnHeight	= abs(mnuBtnTop - mnuBtnBot) / nMnuBtns;

const float				camTilt		= 0.15f;				//Looks nicer, stronger 3D effect
const float				camYShift	= -1.5f;				//To compensate for tilt

const array<float,3>	blockPos	= {0.0f, 0.0f, 0.0f};

M3DMatrix44f			mCamera;							//Handy to have it in global namespace
M3DMatrix44f			mOrtho;
M3DMatrix44f			mIdentity;



//////////////////////////////////////////////////////////////////////////////////////////

//Function primitives:

void setup();											//One-time setup function
void shutdownRC();										//One-time shutdown function
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void checkInput();										//Checks if any relevant input has been registred
void menuClick();
void optionsClick();
void menuKey();
void playClick();
void playKey();
void alphaRender();
void survivalRender();
void menuRender();
void optionsRender();

//////////////////////////////////////////////////////////////////////////////////////////

//Function definitions:

//One-time setup function:
void setup()
{
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
	customShaders.initADSFrag();
	customShaders.initUI();

	//Set initial camera position:
	cameraFrame.MoveForward(bgZpos);
	cameraFrame.MoveUp(camYShift);
	cameraFrame.RotateWorld(camTilt, 1.0f, 0.0f, 0.0f);

	//Initiate background:
	bg.init(bgWidth, bgHeight, 0.0f);

	//Initiate UI elements:
	restartInfo.init(17.0f, 43.0f, 83.0f, 53.0f, 0.5f, "Assets/FONT_BLOCK.tga");
	menuBG.init(0.0f, 0.0f, 100.0f, 100.0f, 0.0f, "Assets/menu/MenuBackground.tga");
	alphaButton.init(mnuBtnLeft, mnuBtnTop - mnuBtnHeight, mnuBtnRight, mnuBtnTop, 0.0f, "Assets/button_alpha.tga");
	survivalButton.init(mnuBtnLeft, mnuBtnTop - 2 * mnuBtnHeight, mnuBtnRight, mnuBtnTop - mnuBtnHeight, 0.0f, "Assets/button_survival.tga");
	optionsButton.init(mnuBtnLeft, mnuBtnTop - 3 * mnuBtnHeight, mnuBtnRight, mnuBtnTop - 2 * mnuBtnHeight, 0.0f, "Assets/button_options.tga");
	exitButton.init(mnuBtnLeft, mnuBtnBot, mnuBtnRight, mnuBtnTop - 3 * mnuBtnHeight, 0.0f, "Assets/button_quit.tga");

	optSoundToggle.init(mnuBtnLeft, mnuBtnBot, mnuBtnRight, mnuBtnTop - 3 * mnuBtnHeight, 0.0f, "Assets/menu/Sound_normal.tga");

	font.init("Assets/fatLato.tga");

	//More initiations below here ...
	blocks.init(bgWidth, bgHeight, 0.0f);

	music.init("Assets/Lobby2.wav");
	music.play();

	menuClickSound.init("Assets/douf.wav");

	gameTime.resetTime();

}

void shutdownRC()
{
	glutDestroyWindow(glutGetWindow());

	alphaButton.clearTexture();
	survivalButton.clearTexture();
	optionsButton.clearTexture();
	exitButton.clearTexture();

	optSoundToggle.clearTexture();
	optBackButton.clearTexture();

	font.clearTexture();

	music.clearSound();
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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if (Globals::state == Globals::STATE_MENU)				{
		menuRender();
	} else if (Globals::state == Globals::STATE_OPTIONS)	{
		optionsRender();
	} else if (Globals::state == Globals::STATE_ALPHA)		{
		alphaRender();
	} else if (Globals::state == Globals::STATE_SURVIVAL)	{
		survivalRender();
	}

	glutSwapBuffers();
	glutPostRedisplay();

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

		if (Globals::nBlocks <= 0 && Globals::state == Globals::STATE_ALPHA)
		{
			finalTime = gameTime.getElapsedTime();	//Save final time to finalTime
		} else if (Globals::nBlocks <= 0 && Globals::state == Globals::STATE_SURVIVAL && Globals::lives > 0) {
			Globals::speed *= 1.05f;
			blocks.sendWave(5, Globals::speed);
		}
	}
}

void playKey()
{
	if (Input::pressedKey == ' ' && Globals::state == Globals::STATE_ALPHA && Globals::nBlocks <= 0) {
		blocks.sendWave(10, 0.0f);
		gameTime.resetTime();
	} else if (Input::pressedKey == ' ' && Globals::state == Globals::STATE_SURVIVAL && Globals::lives <= 0) {
		Globals::lives = 3;
		Globals::score = 0;
		Globals::speed = 0.002f;
		blocks.sendWave(5, Globals::speed);
		gameTime.resetTime();
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

	if (exitButton.isClicked(clickPos)) {
		//menuClickSound.play();
		shutdownRC();
		exit(0);
	} else if (optionsButton.isClicked(clickPos)) {
		//menuClickSound.play();
		Globals::state = Globals::STATE_OPTIONS;
	} else if (survivalButton.isClicked(clickPos)) {
		//menuClickSound.play();
		Globals::state = Globals::STATE_SURVIVAL;
		Globals::lives = 3;
		Globals::score = 0;
		Globals::speed = 0.003f;
		blocks.sendWave(5, Globals::speed);
		gameTime.resetTime();
	} else if (alphaButton.isClicked(clickPos)) {
		//menuClickSound.play();
		Globals::state = Globals::STATE_ALPHA;
		blocks.sendWave(10, 0.0f);
		gameTime.resetTime();
	}
}

void optionsClick() {
	// Clicking position in UI-coords
	array<float,2> clickPos = Input::getUICoords(Input::clickPos[0], Input::clickPos[1]);

	if (optSoundToggle.isClicked(clickPos)) {
		Globals::state = Globals::STATE_MENU;
	}
}

void menuKey()
{
	if (Input::pressedKey == 27) { //Escape
		shutdownRC();
		exit(0);
	} else if (Input::pressedKey == 'o') {
		Globals::state = Globals::STATE_MENU;
	}
}

void checkInput()
{
	if (Input::hasClicked) {
		Input::hasClicked = false;
		if (Globals::state == Globals::STATE_ALPHA || Globals::state == Globals::STATE_SURVIVAL) {
			playClick(); 
		} else if (Globals::state == Globals::STATE_MENU) {
			menuClick();
		} else if (Globals::state == Globals::STATE_OPTIONS) {
			optionsClick();
		}
	}
	if (Input::hasPressed) {
		Input::hasPressed = false;
		if (Globals::state == Globals::STATE_ALPHA || Globals::state == Globals::STATE_SURVIVAL) {
			playKey();
		} else if (Globals::state == Globals::STATE_MENU || Globals::state == Globals::STATE_OPTIONS) {
			menuKey();
		}
	}
	if (Input::hasPressedSpecial) {
		Input::hasPressedSpecial = false;
	}
}

void alphaRender()
{
	//Lighting variables:
	static GLfloat vLightPos[] = {-500.0f, 50.0f, 100.0f};
	static GLfloat vAmbient[] = {0.2f, 0.2f, 0.2f, 0.2f};
			
	//Beginning push:
	modelViewStack.PushMatrix();
	
	//Camera matrix:
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewStack.PushMatrix(mCamera);
	
	//Calc light pos in eye coords:
	static GLfloat vLightEyePos[3];
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
	
	static char gtime[64];
	static float tw = 0.0f;
	static char *grade;

	//Render a thing in the thing on the thing:
	if (Globals::nBlocks <= 0) {
		restartInfo.draw(uiPipeline, gltShaderManager);
		tw = 80.0f;
		if (finalTime <= 2.50) {
			grade = "GODLIKE!!!";
		} else if (finalTime <= 3.00) {
			grade = "MASTER!";
		} else if (finalTime <= 4.5) {
			grade = "REGULAR";
		} else {
			grade = "GRANNY ...";
		}
		sprintf_s(gtime, "You did it in: %.2f sec | Rating: %s", finalTime, grade);
	} else if (Globals::nBlocks > 0) {
		tw = 20.0f;
		sprintf_s(gtime, "%.2f sec", gameTime.getElapsedTime());
	}
	
	font.showText(gtime, 1.0f, 90.0f, tw, 6.0f, gltShaderManager, uiPipeline);
}

void survivalRender()
{
	//Lighting variables:
	static GLfloat vLightPos[] = {-500.0f, 50.0f, 100.0f};
	static GLfloat vAmbient[] = {0.2f, 0.2f, 0.2f, 0.2f};
			
	//Beginning push:
	modelViewStack.PushMatrix();
	
	//Camera matrix:
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewStack.PushMatrix(mCamera);
	
	//Calc light pos in eye coords:
	static GLfloat vLightEyePos[3];
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
	
	static char gtime[64];
	static char gscore[16];
	static char glives[16];
	static float tw = 0.0f;

	//Render a thing in the thing on the thing:
	if (Globals::nBlocks > 0) {
		tw = 20.0f;
		sprintf_s(gtime, "%.2f sec", gameTime.getElapsedTime());
	} else if (Globals::lives <= 0) {
		restartInfo.draw(uiPipeline, gltShaderManager);
		tw = 75.0f;
		sprintf_s(gtime, "You survived for: %.2f sec", gameTime.getElapsedTime()); // Get the final time Jonas?
	} else if (Globals::lives > 0 && Globals::nBlocks <= 0) {
		Globals::speed *= 1.1f;
		tw = 20.0f;
		blocks.sendWave(5, Globals::speed);
		sprintf_s(gtime, "%.2f sec", gameTime.getElapsedTime());
	}

	sprintf_s(gscore, "Score: %i", Globals::score);
	sprintf_s(glives, "Lives : %i", Globals::lives);
	
	font.showText(gtime, 1.0f, 90.0f, tw, 6.0f, gltShaderManager, uiPipeline);
	font.showText(gscore, 1.0f, 1.0f, 16.0f, 6.0f, gltShaderManager, uiPipeline);
	font.showText(glives, 80.0f, 1.0f, 16.0f, 6.0f, gltShaderManager, uiPipeline);
}

void menuRender()
{
	menuBG.draw(uiPipeline, gltShaderManager);
	alphaButton.draw(uiPipeline, gltShaderManager);
	survivalButton.draw(uiPipeline, gltShaderManager);
	optionsButton.draw(uiPipeline, gltShaderManager);
	exitButton.draw(uiPipeline, gltShaderManager);
}

void optionsRender() {
	font.showText("Options", 1.0f, 90.0f, 3.0f*3, 8.0f, gltShaderManager, uiPipeline);
	optSoundToggle.draw(uiPipeline, gltShaderManager);
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