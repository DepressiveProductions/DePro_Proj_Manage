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
	
		void initADS();
		void useADS(M3DVector4f vDiffuseColor, M3DVector4f vLightEyePos, GLGeometryTransform tPipeline);

	private:
		//Shaders:
		GLuint ADS;

		//Uniform locators:
		GLint locColor, locLight, locMVP, locMV, locNM;
};

