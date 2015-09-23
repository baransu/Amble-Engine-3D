#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <SOIL.h>
#include <vector>

#include "Engine\Source\Camera.h"
#include "Engine\Source\Shader.h"
#include "Engine\Source\Model.h"
#include "Engine\Source\Skybox.h"
#include "Engine\Source\PointLight.h"
#include "Engine\Source\utils\FPS.h"
#include "Engine\Source\utils\Time.h"
#include "Engine\Source\utils\Loader.h"

#include <QGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <QTime>
#include <qelapsedtimer.h>

class GlWidget : public QGLWidget
{
	Q_OBJECT

public:
	explicit GlWidget(QWidget *parent = 0);
	~GlWidget();

	int WIDTH = 1280;
	int HEIGHT = 720;
	Camera* camera();
	QElapsedTimer timer1;

	float deltaTime = 0;
	
	void initializeGL();
	//void updateGL();
	void paintGL();
	void resizeGL(int w, int h);

private:

	long lastFrame = 0;

	QTimer timer;

	Shader* simpleShader();
	Skybox* skybox();
	PointLight* lamp();
	Shader* shader();
	Shader* fbShader();
	Model* mouseModel();
	
	GLuint VAO, VBO;
	GLuint framebuffer;
	glm::vec3 lightPos;
	GLuint diffuseMap, specularMap, normalMap;
	GLuint textureColorbuffer;
	GLuint quadVAO, quadVBO;
	GLuint cubemapTexture;
	
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	GLuint loadTexture(GLchar* path, GLboolean alpha);
	GLuint loadCubemap(std::vector<const GLchar*> faces);
	
};
#endif // GLWIDGET_H
