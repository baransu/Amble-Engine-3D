#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"

class Skybox
{
public:
	//Skybox(const Shader &shader);
	~Skybox();

	void draw();

private:
	void setup();
	Shader m_Shader;
	//GLfloat m_QuadVertices[] = {
	//	// Positions   // TexCoords
	//	-1.0f,  1.0f, 0.0f, 1.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f,

	//	-1.0f,  1.0f, 0.0f, 1.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f,
	//	1.0f,  1.0f, 1.0f, 1.0f
	//};
};

