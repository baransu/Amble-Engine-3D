#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <array>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

class Quad
{
public:
	Quad(const char* vertexPath, const char* fragmentPath, const char* depthVertexShaderPath, const char* depthFragmentShaderPath, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
	Quad();
	~Quad();

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	void draw(Camera &camera, const GLuint &diffuseMap, int lightCount, const glm::vec3 lightPositions[], const glm::vec3 lightColors[], const glm::mat4 &lightSpaceMatrix, const GLuint &shadowMap);
	void drawDepth(glm::mat4 lightSpaceMatrix);

private:
	
	Shader m_Shader;
	Shader m_DepthShader;
	GLuint m_VAO, m_VBO;
	void setup();
};

