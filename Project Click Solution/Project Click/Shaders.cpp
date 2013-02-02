#include "Shaders.h"


Shaders::Shaders(void) {}
Shaders::~Shaders(void) {}

//Diffuse lighting, per vertex
void Shaders::initDiffVert()
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

void Shaders::useDiffVert(M3DVector4f vDiffuseColor, M3DVector3f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(diffVert);
	glUniform4fv(vDiffColor, 1, vDiffuseColor);
	glUniform3fv(vDiffLight, 1, vLightEyePos);
	glUniformMatrix4fv(vDiffMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(vDiffMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(vDiffNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}


//ADS lighting, per vertex
void Shaders::initADSVert()
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

void Shaders::useADSVert(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline)
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
void Shaders::initADSFrag()
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

void Shaders::useADSFrag(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline)
{
	glUseProgram(ADSFrag);
	glUniform4fv(fADSambColor, 1, vAmbientColor);
	glUniform4fv(fADSspecColor, 1, vSpecularColor);
	glUniform4fv(fADSdiffColor, 1, vDiffuseColor);
	glUniformMatrix4fv(fADSMVP, 1, GL_FALSE, tPipeline.GetModelViewProjectionMatrix());
	glUniformMatrix4fv(fADSMV, 1, GL_FALSE, tPipeline.GetModelViewMatrix());
	glUniformMatrix3fv(fADSNM, 1, GL_FALSE, tPipeline.GetNormalMatrix());
}

void Shaders::initUI()
{
	//Load and compile the shader programs:
	UIShader = gltLoadShaderPairWithAttributes("Shaders/UIShader.vp", "Shaders/UIShader.fp", 
												2, GLT_ATTRIBUTE_VERTEX, "vVertex", 
												GLT_ATTRIBUTE_TEXTURE0, "vTexCoord");
                 
	//Locate uniforms:
	UIMVP = glGetUniformLocation(UIShader, "mvpMatrix");
	UITex = glGetUniformLocation(UIShader, "rectangleImage");
}

void Shaders::useUI(M3DMatrix44f mProj, GLuint texture)
{
	glUseProgram(UIShader);
	glUniform1i(UITex, 0);
	glUniformMatrix4fv(UIMVP, 1, GL_FALSE, mProj);
	glBindTexture(GL_TEXTURE_RECTANGLE, texture);
}