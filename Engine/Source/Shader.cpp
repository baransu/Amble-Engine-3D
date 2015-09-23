#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	m_VertexPath = vertexPath;
	m_FragmentPath = fragmentPath;
	m_ShaderProgram = load();
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
}

GLuint Shader::load()
{
	//create
	GLuint shaderProgram = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//load source
	std::string vertexSourceString = File::read_file(m_VertexPath);
	std::string fragmentSourceString = File::read_file(m_FragmentPath);
	const char* vertexShaderSource = vertexSourceString.c_str();
	const char* fragmentShaderSource = fragmentSourceString.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//error log veriables
	GLint success;
	GLchar infoLog[512];

	//compiling
	//vertex
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(vertexShader);
	}

	//fragment
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(fragmentShader);
	}

	//linking shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	//linking error check
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(shaderProgram);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLint Shader::getUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(m_ShaderProgram, name);
}

void Shader::setUniformMatrix4fv(const GLchar* name, glm::mat4 value)
{
	glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform4f(const GLchar* name, float x, float y, float z, float w)
{
	glUniform4f(this->getUniformLocation(name), x, y, z, w);
}

void Shader::setUniform3fv(const GLchar* name, int count, const GLfloat* value)
{
	glUniform3fv(this->getUniformLocation(name), count, value);
}

void Shader::setUniform3f(const GLchar* name, float x, float y, float z)
{
	glUniform3f(this->getUniformLocation(name), x, y, z);
}

void Shader::setUniform1f(const GLchar* name, float value)
{
	glUniform1f(this->getUniformLocation(name), value);
}

void Shader::setUniform1i(const GLchar* name, int value)
{
	glUniform1i(this->getUniformLocation(name), value);
}

void Shader::use()
{ 
	glUseProgram(m_ShaderProgram);
}

void Shader::disable() const
{
	glUseProgram(0);
}


