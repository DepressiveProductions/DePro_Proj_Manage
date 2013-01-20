//STATES:
//#define HOUSE_DEBUG
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
#include <vector>
#include <thread>
#include <array>

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
#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_TITLE "Project: Management - Prototype"

array<float, 3> actualPos;
array<float, 3> lastPos;
bool destroyMode = true;

//Some important objects:
MyShaderManager emilShaders;
GLShaderManager shaderManager;
GLGeometryTransform tPipeline;
GLMatrixStack projectionStack;
GLMatrixStack modelViewStack;
GLFrustum viewFrustum;
GLFrame cameraFrame;
Input in;

using std::vector;
using std::array;

vector< House * > buildingTypes;
Floor ground;
Grid hlGrid; // Highlight grid
array<float,3> clickPos;

void setupRC();											//One-time setup function (RC = Rendering Context)
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void handleInput();										//Handles input ...

void setupRC()
{
	//Background:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth testing, multisampling, blending:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	//Initialize stock shaders from GLTools:
	shaderManager.InitializeStockShaders();

	//Experimental first self-written shaders:
	emilShaders.initDiffVert();
	emilShaders.initADSVert();
	//emilShaders.initADSFrag();
	
	//Move cam into place:
	cameraFrame.MoveForward(-20.0f);
	cameraFrame.MoveUp(-3.5f);
	cameraFrame.RotateWorld(0.15f, 1.0f, 0.0f, 0.0f);

	//Initiate ground:
	ground.init(0.0f, 0.0f, 0.0f, 40, 10, C_RAD);

	//Initiate class for handeling some stuff:
	hlGrid.init(0.0f, 0.0f, 0.0f, C_RAD);

	//Initate a type of building:
	M3DVector4f baracksShine = {0.5f, 0.5f, 0.5f, 1.0};
	M3DVector4f baracksColor = {0.3f, 0.3f, 0.3f, 1.0f};
	buildingTypes.push_back(new House());
	buildingTypes[0]->init(C_RAD, baracksShine, baracksColor);

	//Spawn a test building:
	vector<array<float, 3>> vInitialBPos;
	array<float,3> initialBPos1 = {-3.5f, 0.5f, 0.0f};
	array<float,3> initialBPos2 = {-1.5f, -0.5f, 0.0f};
	hlGrid.boxActivation(initialBPos1, initialBPos2);		
	hlGrid.getSquarePositions(vInitialBPos);
	buildingTypes[0]->create(vInitialBPos);
	hlGrid.deactivateAllSquares();
	vInitialBPos.clear();
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

	//Buildings:
	for (unsigned int i = 0 ; i < buildingTypes.size() ; i++)
		buildingTypes[i]->drawAll(emilShaders, tPipeline, vLightEyePos, modelViewStack, vAmbient);

	//End cam push:
	modelViewStack.PopMatrix();

	//End pop:
	modelViewStack.PopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();

	// Input
	handleInput();
}

void handleInput()
{	
	// Exit
	if (in.keyPressed(sf::Keyboard::Escape))
		exit(0);
}

//Mouseclicking:
void clickFunc(int key, int state, int x, int y)
{
	//If LMB is clicked:
	if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		//Click away a building:
		if (destroyMode)
		{
			bool buildingFound = false;
			in.getCursor3(x, y, clickPos, cameraFrame, projectionStack);
			for (unsigned int bt = 0 ; bt < buildingTypes.size() ; bt++) //Loop through types
			{
				for (unsigned int b = 0 ; b < buildingTypes[bt]->buildings.size() ; b++) //Loop through builings in type
				{
						//If click on building:
						if (clickPos[0] >= buildingTypes[bt]->buildings[b]->getWalls()[0] 
							&& clickPos[0] <= buildingTypes[bt]->buildings[b]->getWalls()[1] 
							&& clickPos[1] >= buildingTypes[bt]->buildings[b]->getWalls()[2] 
							&& clickPos[1] <= buildingTypes[bt]->buildings[b]->getWalls()[3])
						{
							buildingTypes[bt]->removeBuilding(b);
							buildingFound = true;
							break;
						}
				}
				if (buildingFound) break;
			}
				
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
	glutMouseFunc(clickFunc);

	GLenum result = glewInit();
	if(result != GLEW_OK)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(result));
		return 1;
	}
	
	//Setup stuff and start looping:
	setupRC();
	glutMainLoop();
	system("PAUSE");
	return 0;
}