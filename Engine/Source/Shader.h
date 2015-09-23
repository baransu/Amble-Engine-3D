#pragma once

#include <iostream>
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "utils/File.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader();
	~Shader();

	void setUniformMatrix4fv(const GLchar* name, glm::mat4 value);
	void setUniform3fv(const GLchar* name, int count, const GLfloat* value);
	void setUniform4f(const GLchar* name, float x, float y, float z, float w);
	void setUniform3f(const GLchar* name, float x, float y, float z);
	void setUniform1f(const GLchar* name, float value);
	void setUniform1i(const GLchar* name, int value);

	void use();
	void disable() const;
	GLuint m_ShaderProgram;
	GLuint load();
private:

	GLint getUniformLocation(const GLchar* name);
	
	const char* m_VertexPath;
	const char* m_FragmentPath;
};

