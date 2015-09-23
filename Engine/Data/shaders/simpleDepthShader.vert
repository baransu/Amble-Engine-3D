// shadertype=<glsl>
#version 330 core
  
//input
layout (location = 0) in vec3 position;

//uniform
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
	gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}