#include "Font.h"

Font::Font(void) {}
Font::~Font(void) {}

void Font::init()
{
	/**/
	char ttf_buffer[1<<20];				// One of theese
	unsigned char temp_bitmap[512*512]; // provoke stack overflow
	unsigned char data = fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/times.ttf", "rb"));
	stbtt_BakeFontBitmap(&data, 0, 32.0f, temp_bitmap, 512, 512, 32, 96, cdata);
	// can free ttf_buffer at this point
	
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/**/
	
	vColor[0] = 0.0f;
	vColor[1] = 0.0f;
	vColor[2] = 1.0f;
	vColor[3] = 1.0f;
}



void Font::showText(char *text, float x, float y,
					float width, float height, GLShaderManager &sm,
					GLGeometryTransform tp)
{
	float z = 0.0f;

	/**/
	// assume orthographic projection with units = screen pixels, origin at top left
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	//glBegin(GL_QUADS);

	GLBatch tempBatch;
	tempBatch.Begin(GL_TRIANGLE_FAN, 512, 1);
   
	stbtt_aligned_quad q;

	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);	//1=opengl,0=old d3d
			
			/*glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y0);
			glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y0);
			glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y1);
			glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y1);*/
			
			// Lower left hand corner
			tempBatch.MultiTexCoord2f(0, q.s0, q.t1);
			tempBatch.Vertex3f(q.x0, q.y0, z);
		
			// Upper left hand corner
			tempBatch.MultiTexCoord2f(0, q.s0, q.t0);
			tempBatch.Vertex3f(q.x0, q.y1, z);  
		
			// Upper right hand corner
			tempBatch.MultiTexCoord2f(0, q.s1, q.t0);
			tempBatch.Vertex3f(q.x1, q.y1, z);
		
			// Lower right hand corner
			tempBatch.MultiTexCoord2f(0, q.s1, q.t1);
			tempBatch.Vertex3f(q.x1, q.y0, z);
		}
		++text;
	}
	//glEnd();
	tempBatch.End();

	/**/

	/*
	GLBatch tempBatch;
	tempBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	
	// Lower left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	tempBatch.Vertex3f(x, y, z);

	// Upper left hand corner
	tempBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
	tempBatch.Vertex3f(x, y + height, z);  

	// Upper right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
	tempBatch.Vertex3f(x + width, y + height, z);

	// Lower right hand corner
	tempBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
	tempBatch.Vertex3f(x + width, y, z);

	tempBatch.End();
	*/

	draw(tempBatch, sm, tp);
}

void Font::draw(GLBatch &ba, GLShaderManager &sm, GLGeometryTransform tp)
{
	sm.UseStockShader(GLT_SHADER_FLAT, tp.GetModelViewProjectionMatrix(), vColor);
	ba.Draw();
}

void Font::clearTexture()
{
	//glDeleteTextures(1, uiTexture);
}