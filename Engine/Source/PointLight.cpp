#include "PointLight.h"

PointLight::PointLight(const char* vertexShaderPath, const char* fragmentShaderPath) : m_Shader(vertexShaderPath, fragmentShaderPath), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), color(1.0f)
{
	this->setup();
}

PointLight::~PointLight()
{
}

void PointLight::draw(Camera camera)
{
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	this->m_Shader.use();
	glm::mat4 model;
	model = glm::translate(model, this->position);
	model = glm::scale(model, this->scale);
	this->m_Shader.setUniformMatrix4fv("model", model);
	this->m_Shader.setUniform3f("lightColor", this->color.x, this->color.y, this->color.z);

	this->m_Shader.setUniformMatrix4fv("view", camera.getViewMatrix());
	this->m_Shader.setUniformMatrix4fv("projection", camera.getProjectionMatrix());
	glBindVertexArray(this->m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	this->m_Shader.disable();
}

void PointLight::setup()
{
	const static GLfloat vertices[108] = {
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}
