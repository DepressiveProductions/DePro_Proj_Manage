#version 330

//Input:
smooth in vec4 vVaryingColor;

//Output:
//out vec4 vFragColor;

void main(void)
{
	gl_FragColor = vVaryingColor;
	//vFragColor = vVaryingColor;
}