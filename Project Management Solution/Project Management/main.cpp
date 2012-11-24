//STATES:
//#define HOUSE_DEBUG
//#define TRIANGLE_DEBUG

//Includes:
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <GLFrame.h>
#include <GLBatch.h>

#include <math.h>
#include <ctime>

#include "House.h"
#include "Button.h"
#include "Grid.h"
#include "Floor.h"
#include "input.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#define W_WIDTH 800
#define W_HEIGHT 600
#define W_TITLE "Project: Management - Prototype"
float camSpeed = 0.1f;

//Some important objects:
GLShaderManager shaderManager;
GLGeometryTransform tPipeline;
GLMatrixStack projectionStack;
GLMatrixStack modelViewStack;
GLFrustum viewFrustum;
GLFrame cameraFrame;

House baracks;
Floor ground;
Grid stdGrid;
Input mainInput;
Button buildButton;

#ifdef TRIANGLE_DEBUG
GLBatch testBatch;
#endif

#ifdef HOUSE_DEBUG
House testHouse;
#endif

void setupRC();							//One-time setup function (RC = Rendering Context)
void changeSize(int w, int h);			//Runs everytime the window 'changes size', for example when the window is created
void renderScene();						//Basic glutfunc for rendering stuff. Runs every frame
void input();							//Handling input since 1962

void setupRC()
{
	//BG:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth testing so things won't look effed up:
	glEnable(GL_DEPTH_TEST);

	//Initialize stock shaders from GLTools:
	shaderManager.InitializeStockShaders();
	
	//Move cam back:
	cameraFrame.MoveForward(-5.0f);

	#ifdef HOUSE_DEBUG
	M3DVector3f pos = {4.0f, 0.0f, -5.0f};
	M3DVector3f pos2 = {10.0f, -20.0f, -5.0f};
	GLfloat radius = 1.0f;
	testHouse.init(radius);
	testHouse.create(pos, pos2, 20);
	#endif

	//Init debug batches:
	#ifdef TRIANGLE_DEBUG
	testBatch.Begin(GL_TRIANGLES, 3);
		testBatch.Vertex3f(1.0f, -1.0f, 0.0f);
		testBatch.Vertex3f(0.0f, 1.0f, 0.0f);
		testBatch.Vertex3f(-1.0f, -1.0f, 0.0f);
	testBatch.End();
	#endif

	ground.init(40, 40, 40);
	stdGrid.initGrid(40, -20, -20, 40, 40);
	baracks.init(1.0f);
	buildButton.init(20, 50, 128, 32, "Assets/button_build_128x32.tga");
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

	//Get input:
	input();

	//Camera matrix:
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewStack.PushMatrix(mCamera);

	//Light source:
	static M3DVector4f vLightPos = {0.0f, 5.0f, 0.0f, 1.0f};
	static M3DVector4f vLightEyePos;
	m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

	//Draw debug batches:
	#ifdef TRIANGLE_DEBUG
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vDarkRed);
	testBatch.Draw();
	#endif

	//House debug drawing:
	#ifdef HOUSE_DEBUG
	testHouse.draw(shaderManager, tPipeline, vLightEyePos, modelViewStack);
	#endif

	//End cam push:
	modelViewStack.PopMatrix();

	//End pop:
	modelViewStack.PopMatrix();
	
	buildButton.draw(W_WIDTH, W_HEIGHT);

	glutSwapBuffers();
	glutPostRedisplay();
}

void input()
{	
	short keyLeft = GetKeyState(VK_LEFT);
    bool pushLeft = keyLeft < 0;
    if (pushLeft == true)
    {
		cameraFrame.MoveRight(camSpeed);
    }
 
	short keyRight = GetKeyState(VK_RIGHT);
	bool pushRight = keyRight < 0;
	if (pushRight == true)
	{
		cameraFrame.MoveRight(-camSpeed);
	}
 
	short keyUp = GetKeyState(VK_UP);
	bool pushUp = keyUp < 0;
	if (pushUp == true)
	{
		cameraFrame.MoveUp(camSpeed);
	}
 
	short keyDown = GetKeyState(VK_DOWN);
	bool pushDown = keyDown < 0;
	if (pushDown == true)
	{
		cameraFrame.MoveUp(-camSpeed);
	}      
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	//Initiate glut and create a window:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow(W_TITLE);

	//Send funcs to gluttis:
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);

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
	return 0;
}