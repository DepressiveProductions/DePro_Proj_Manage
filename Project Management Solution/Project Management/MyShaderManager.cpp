#include "MyShaderManager.h"


MyShaderManager::MyShaderManager(void)
{
}


MyShaderManager::~MyShaderManager(void)
{
}

void MyShaderManager::initDiffVert()
{
	//Load and compile the shader programs:
	diffVert = gltLoadShaderPairWithAttributes("Shaders/diffuse_vertex.vp", "Shaders/diffuse_vertex.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");
	
	//Locate uniforms:
	diffColor = glGetUniformLocation(diffVert, "diffuseColor");
	diffLight = glGetUniformLocation(diffVert, "vLightPosition");
	diffMVP = glGetUniformLocation(diffVert, "mvpMatrix");
	diffMV = glGetUniformLocation(diffVert, "mvMatrix");
	diffNM = glGetUniformLocation(diffVert, "normalMatrix");
}

void MyShaderManager::useDiffVert(M3DVector4f vDiffuseColor, M3DVector3f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(diffVert);
	glUniform4fv(diffColor, 1, vDiffuseColor);
	glUniform3fv(diffLight, 1, vLightEyePos);
	glUniformMatrix4fv(diffMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(diffMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(diffNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}



void MyShaderManager::initADSVert()
{
	//Load and compile the shader programs:
	ADSVert = gltLoadShaderPairWithAttributes("Shaders/ADS_vertex.vp", "Shaders/ADS_vertex.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");

	//Locate uniforms:
	ADSambColor = glGetUniformLocation(ADSVert, "ambientColor");
	ADSspecColor = glGetUniformLocation(ADSVert, "specularColor");
	ADSdiffColor = glGetUniformLocation(ADSVert, "diffuseColor");
	ADSLight = glGetUniformLocation(ADSVert, "vLightPosition");
	ADSMVP = glGetUniformLocation(ADSVert, "mvpMatrix");
	ADSMV = glGetUniformLocation(ADSVert, "mvMatrix");
	ADSNM = glGetUniformLocation(ADSVert, "normalMatrix");
}

void MyShaderManager::useADSVert(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(ADSVert);
	glUniform4fv(ADSambColor, 1, vAmbientColor);
	glUniform4fv(ADSspecColor, 1, vSpecularColor);
	glUniform4fv(ADSdiffColor, 1, vDiffuseColor);
	glUniformMatrix4fv(ADSMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(ADSMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(ADSNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}