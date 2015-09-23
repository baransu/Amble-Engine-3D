#include "Quad.h"


Quad::Quad(const char* vertexPath, const char* fragmentPath, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : m_Shader(vertexPath, fragmentPath)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->setup();
}

Quad::~Quad()
{
}

void Quad::draw(Camera &camera, const GLuint &diffuseMap, int lightCount, const glm::vec3 lightPositions[], const glm::vec3 lightColors[])
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	this->m_Shader.use();
	glDisable(GL_CULL_FACE);

	glm::mat4 model;
	model = glm::translate(model, this->position);
	model = glm::scale(model, this->scale);
	//GLfloat angle = 30;
	model = glm::rotate(model, (GLfloat)-90.0f, this->rotation);
	this->m_Shader.setUniformMatrix4fv("model", model);

	this->m_Shader.setUniformMatrix4fv("view", camera.getViewMatrix());
	
	this->m_Shader.setUniformMatrix4fv("projection", camera.getProjectionMatrix());
	this->m_Shader.setUniform3f("lightColors[0]", 0.3f, 0.3f, 0.3f);
	this->m_Shader.setUniform3f("lightColors[1]", 0.5f, 0.5f, 0.5f);
	this->m_Shader.setUniform3f("lightColors[2]", 0.7f, 0.7f, 0.7f);
	this->m_Shader.setUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
	this->m_Shader.setUniform1f("material.shininess", 32);
	this->m_Shader.setUniform3fv("lightPositions", lightCount, &lightPositions[0][0]);
	this->m_Shader.setUniform3fv("lightColors", lightCount, &lightColors[0][0]);
	this->m_Shader.setUniform3f("viewPos", camera.position.x, camera.position.y, camera.position.z);

	//bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	
	//draw
	glBindVertexArray(this->m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	//unbind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	this->m_Shader.disable();

}

void Quad::setup()
{
	// positions
	glm::vec3 pos1(-1.0, 1.0, 0.0);
	glm::vec3 pos2(-1.0, -1.0, 0.0);
	glm::vec3 pos3(1.0, -1.0, 0.0);
	glm::vec3 pos4(1.0, 1.0, 0.0);
	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 0.0, 1.0);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// - triangle 1
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// - triangle 2
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);

	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);

	GLfloat quadVertices[] = {
		// Positions            // normal         // TexCoords  // Tangent                          // Bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	// Setup plane VAO
	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));

	this->m_Shader.use();
	this->m_Shader.setUniform1i("material.diffuse", 0);
	this->m_Shader.disable();

}
