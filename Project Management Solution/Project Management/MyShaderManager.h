#pragma once

#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>

class MyShaderManager
{
	public:
		MyShaderManager(void);
		~MyShaderManager(void);
	
		//Diffuse lighting, per vertex:
		void initDiffVert();
		void useDiffVert(M3DVector4f vDiffuseColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline);

		//ADS lighting, per vertex:
		void initADSVert();
		void useADSVert(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline);

		//ADS lighting, per fragment:
		void initADSFrag();
		void useADSFrag(M3DVector4f vDiffuseColor, M3DVector4f vAmbientColor, M3DVector4f vSpecularColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline);

	private:
		//Shaders:
		GLuint diffVert;
		GLuint ADSVert;
		GLuint ADSFrag;

		//Uniform locators:
		GLint vDiffColor, vDiffLight, vDiffMVP, vDiffMV, vDiffNM;
		GLint vADSambColor, vADSspecColor, vADSdiffColor, vADSLight, vADSMVP, vADSMV, vADSNM;
		GLint fADSambColor, fADSspecColor, fADSdiffColor, fADSLight, fADSMVP, fADSMV, fADSNM;
};

