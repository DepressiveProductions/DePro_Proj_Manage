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

	private:
		//Shaders:
		GLuint diffVert;
		GLuint ADSVert;

		//Uniform locators:
		GLint diffColor, diffLight, diffMVP, diffMV, diffNM;
		GLint ADSambColor, ADSspecColor, ADSdiffColor, ADSLight, ADSMVP, ADSMV, ADSNM;
};

