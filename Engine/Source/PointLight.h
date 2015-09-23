#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <array>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"


class PointLight
{
public:
	PointLight(const char* vertexShaderPath, const char* fragmentShaderPath);
	PointLight();
	~PointLight();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;

	void draw(Camera camera);
	Shader m_Shader;
private: 
	void setup();

	GLuint m_VAO, m_VBO;
};

