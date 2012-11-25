#include "MyShaderManager.h"


MyShaderManager::MyShaderManager(void)
{
}


MyShaderManager::~MyShaderManager(void)
{
}

void MyShaderManager::initADS()
{
	//Load and compile the shader programs:
	ADS = gltLoadShaderPairWithAttributes("Shaders/simpleADS.vp", "Shaders/simpleADS.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");
	
	//Locate uniforms:
	locColor = glGetUniformLocation(ADS, "diffuseColor");
	locLight = glGetUniformLocation(ADS, "vLightPosition");
	locMVP = glGetUniformLocation(ADS, "mvpMatrix");
	locMV = glGetUniformLocation(ADS, "mvMatrix");
	locNM = glGetUniformLocation(ADS, "normalMatrix");
}

void MyShaderManager::useADS(M3DVector4f vDiffuseColor, M3DVector3f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(ADS);
	glUniform4fv(locColor, 1, vDiffuseColor);
	glUniform3fv(locLight, 1, vLightEyePos);
	glUniformMatrix4fv(locMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(locMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(locNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}