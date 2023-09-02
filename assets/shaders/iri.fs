#version 330 core

#define PI 3.1415926538

in vec3 Position;
in vec3 Normal;

out vec4 outColor;

uniform sampler2D waxTex;
uniform vec3 lightPos;

const vec3 a = vec3(0.5);
const vec3 b = vec3(0.5);
const vec3 c = vec3(1.0);
const vec3 d = vec3(0.0,0.33,0.67);


void main()
{
	float t = dot(Normal,normalize(lightPos - Position));
	outColor = vec4(a + b*cos(2.0*PI*(c*t + d)),1.0);
}