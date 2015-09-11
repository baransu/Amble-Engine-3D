#pragma once

#include <iostream>
#include <GL/glew.h>

#include "utils/File.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

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

