#version 330 core

in vec3 position;
in vec3 normal;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float time;

void main(){
	Position = (model * vec4(position,1.0)).xyz;
	Normal = normalize((inverse(transpose(model)) * vec4(normal,1.0)).xyz);
	float t = (1.0+sin(time*2.0))*0.5;
	gl_Position = proj * view * model * vec4(Position*t + (1-t)*normal,1.0);
}