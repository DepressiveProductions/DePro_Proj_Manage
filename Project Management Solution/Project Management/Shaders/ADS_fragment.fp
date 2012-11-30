#version 330

//Uniforms:
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform vec3 vLightPosition;
uniform mat3 normalMatrix;

//Input:
smooth in vec3 vNormalNew;
smooth in vec3 vPosition3;

//Output:
out vec4 vFragColor;

vec4 vColor;

void main(void)
{
	vec3 vEyeNormal = normalMatrix * vNormalNew;

	vec3 vLightDir = normalize(vLightPosition - vPosition3);

	//Diffuse light:
	float diff = max(0.0, dot(vEyeNormal, vLightDir));
	vColor.rgb = diff * diffuseColor.rgb;

	//Ambient light:
	vColor += ambientColor;

	//Specular light:
	vec3 vReflection = normalize(reflect(-vLightDir, vEyeNormal));
	float spec = max(0.0, dot(vEyeNormal, vReflection));
	if (diff != 0)
	{
		float fSpec = pow(spec, 128.0);
		vColor.rgb += vec3(fSpec, fSpec, fSpec);
	}



	gl_FragColor = vColor;
}