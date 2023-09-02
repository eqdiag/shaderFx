#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 outColor;

uniform vec3 color;
uniform vec3 eye;

void main()
{
	//vec3 normalColor = 0.5*(Normal + vec3(1.));
	vec3 lightDir = normalize(eye - Position);
	vec3 col = max(0.0,dot(lightDir,Normal)) * color + vec3(0.3);
	outColor = vec4(col,1.0);
}