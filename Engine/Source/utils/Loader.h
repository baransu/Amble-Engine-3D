#pragma once

#include <SOIL.h>
#include <vector>

#include <iostream>
#include <string>
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

class Loader
{
public:
	//Loader();
	//~Loader();

	static GLuint loadTexture(GLchar* path, GLboolean alpha)
	{
		//Generate texture ID and load texture data 
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		//glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
};

