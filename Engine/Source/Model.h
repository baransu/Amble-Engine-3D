#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <SOIL.h>

#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLint TextureFromFile(const char* path, std::string directory);

class Model
{
public:
	Model(GLchar* path);
	Model();
	void draw(const Shader &shader);
private:
	//model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

