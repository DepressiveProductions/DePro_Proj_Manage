#include "MyShaderManager.h"


MyShaderManager::MyShaderManager(void)
{
}


MyShaderManager::~MyShaderManager(void)
{
}

//Diffuse lighting, per vertex
void MyShaderManager::initDiffVert()
{
	//Load and compile the shader programs:
	diffVert = gltLoadShaderPairWithAttributes("Shaders/diffuse_vertex.vp", "Shaders/diffuse_vertex.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");
	
	//Locate uniforms:
	vDiffColor = glGetUniformLocation(diffVert, "diffuseColor");
	vDiffLight = glGetUniformLocation(diffVert, "vLightPosition");
	vDiffMVP = glGetUniformLocation(diffVert, "mvpMatrix");
	vDiffMV = glGetUniformLocation(diffVert, "mvMatrix");
	vDiffNM = glGetUniformLocation(diffVert, "normalMatrix");
}

void MyShaderManager::useDiffVert(M3DVector4f vDiffuseColor, M3DVector3f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(diffVert);
	glUniform4fv(vDiffColor, 1, vDiffuseColor);
	glUniform3fv(vDiffLight, 1, vLightEyePos);
	glUniformMatrix4fv(vDiffMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(vDiffMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(vDiffNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}


//ADS lighting, per vertex
void MyShaderManager::initADSVert()
{
	//Load and compile the shader programs:
	ADSVert = gltLoadShaderPairWithAttributes("Shaders/ADS_vertex.vp", "Shaders/ADS_vertex.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");

	//Locate uniforms:
	vADSambColor = glGetUniformLocation(ADSVert, "ambientColor");
	vADSspecColor = glGetUniformLocation(ADSVert, "specularColor");
	vADSdiffColor = glGetUniformLocation(ADSVert, "diffuseColor");
	vADSLight = glGetUniformLocation(ADSVert, "vLightPosition");
	vADSMVP = glGetUniformLocation(ADSVert, "mvpMatrix");
	vADSMV = glGetUniformLocation(ADSVert, "mvMatrix");
	vADSNM = glGetUniformLocation(ADSVert, "normalMatrix");
}

void MyShaderManager::useADSVert(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(ADSVert);
	glUniform4fv(vADSambColor, 1, vAmbientColor);
	glUniform4fv(vADSspecColor, 1, vSpecularColor);
	glUniform4fv(vADSdiffColor, 1, vDiffuseColor);
	glUniformMatrix4fv(vADSMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(vADSMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(vADSNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}

//ADS lighting, per fragment
void MyShaderManager::initADSFrag()
{
	//Load and compile the shader programs:
	ADSFrag = gltLoadShaderPairWithAttributes("Shaders/ADS_fragment.vp", "Shaders/ADS_fragment.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal");

	//Locate uniforms:
	fADSambColor = glGetUniformLocation(ADSFrag, "ambientColor");
	fADSspecColor = glGetUniformLocation(ADSFrag, "specularColor");
	fADSdiffColor = glGetUniformLocation(ADSFrag, "diffuseColor");
	fADSLight = glGetUniformLocation(ADSFrag, "vLightPosition");
	fADSMVP = glGetUniformLocation(ADSFrag, "mvpMatrix");
	fADSMV = glGetUniformLocation(ADSFrag, "mvMatrix");
	fADSNM = glGetUniformLocation(ADSFrag, "normalMatrix");
}

void MyShaderManager::useADSFrag(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(ADSFrag);
	glUniform4fv(fADSambColor, 1, vAmbientColor);
	glUniform4fv(fADSspecColor, 1, vSpecularColor);
	glUniform4fv(fADSdiffColor, 1, vDiffuseColor);
	glUniformMatrix4fv(fADSMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(fADSMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(fADSNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}