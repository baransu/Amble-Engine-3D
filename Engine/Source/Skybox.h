#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <array>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(const char* vertexShaderPath, const char* fragmentShaderPath);
	Skybox();
	~Skybox();

	void draw(Camera &camera, const GLuint &cubemapTexture);

private:
	void setup();

	Shader m_Shader;
	GLuint m_VAO, m_VBO;
};

