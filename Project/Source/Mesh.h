#pragma once

#include <glm.hpp>
#include <sstream>
#include <string>
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <vector>

#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitTangent;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	/*~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}*/
	void draw(const Shader &shader);

	//mesh data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

private:
	void setupMesh();

	//render data
	GLuint VAO, VBO, EBO;
};