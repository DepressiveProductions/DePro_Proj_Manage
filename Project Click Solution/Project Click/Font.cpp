#include "Font.h"


Font::Font(void) {}
Font::~Font(void) {}

void Font::init(const char * imagepath)
{
	GLuint Texture = loadDDS("uvmap.DDS");
	/*GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");*/


	nTextures = texFileNames.size();

	int counter = 0;
	for (auto i = texFileNames.begin() ; i != texFileNames.end(); i++) {
		glGenTextures(1, &texIDs[i->first]);
		glBindTexture(GL_TEXTURE_2D, texIDs[i->first]);
		loadTGATexture(i->second.c_str(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
		//texIDs[i->first] = counter;
		counter++;
	}
}

void Font::showText(std::string text, float x, float y, float width, float height)
{
	if (height == 0.0f) {										//Default aspect ratio
		height = width * 5 / 3;
	} if (x + (width * ((float)text.size() + 1.0f)) > 100.0f) {	//Not outside screen!
		x =  -(width * ((float)text.size() + 1.0f)) - 100.0f;
	}

	vector<letter *> tempVec;

	for (unsigned int i = 0.0f ; i < text.size() ; i++) {
		tempVec.push_back(new letter);
		initiateBatch(tempVec[tempVec.size()-1], x + i*width, y, width, height);
		//tempVec[tempVec.size()-1]->iTexture = texIDs[text[i]];
		tempVec[tempVec.size()-1]->c = text[i];
		allText[text] = tempVec;
	}
}

void Font::removeText(std::string key)
{
	for (auto iterator = allText.begin() ; iterator != allText.end() ; iterator++) {
		if (iterator->first == key) {
			delete &allText[key];
			allText.erase(key);
		}
	}
}

void Font::initiateBatch(letter *l, float x, float y, float width, float height)
{
	l->lBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	z = 0.0f;

	// Lower left hand corner
	l->lBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	l->lBatch.Vertex3f(x, y, z);

	// Upper left hand corner
	l->lBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	l->lBatch.Vertex3f(x, y + height, z);  

	// Upper right hand corner
	l->lBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	l->lBatch.Vertex3f(x + width, y + height, z);

	// Lower right hand corner
	l->lBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	l->lBatch.Vertex3f(x + width, y, z);

	l->lBatch.End();
}

void Font::drawAll(GLGeometryTransform pipeline, GLShaderManager &shaderManager)
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	for (auto iter = allText.begin() ; iter != allText.end() ; iter++) {

		for (unsigned int i = 0 ; i < iter->second.size() ; i++) {
			glBindTexture(GL_TEXTURE_2D, texIDs[iter->second[i]->c]);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, pipeline.GetModelViewProjectionMatrix(), iter->second[i]->iTexture);
			iter->second[i]->lBatch.Draw();
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Font::clearTexture()
{
	glDeleteTextures(nTextures, uiTexture);
}

GLuint Font::loadDDS(const char * imagepath)
{
	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL) 
		return 0; 
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 
	} 

	free(buffer); 

	return textureID;


}