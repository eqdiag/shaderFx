#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 outColor;

uniform sampler2D waxTex;

void main()
{
	vec2 uv = 0.5*(Normal.xy + 1.0);
	outColor = texture(waxTex,uv);
}