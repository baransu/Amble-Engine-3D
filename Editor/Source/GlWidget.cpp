#include "GlWidget.h"

GlWidget::GlWidget(QWidget *parent) : QGLWidget(parent)
	//camera(glm::vec3(0.0f, 0.0f, 3.0f), 72.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f),
	//shader("Data/shaders/normal.vert", "Data/shaders/normal.frag"), lamp("Data/shaders/lampShader.vert", "Data/shaders/lampShader.frag"),
	//mouseModel("Data/models/mouse/mousino.obj"), fbShader("Data/shaders/fb.vert", "Data/shaders/fb.frag"), lightPos(0.0f, 5.0f, 0.0f)
{
	//game loop - 0 interval = real time?
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer.start(16);

	
	timer1.start();

	this->makeCurrent();
	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(glew_status));
	}
}

GlWidget::~GlWidget()
{
}

void GlWidget::initializeGL()
{
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);


	//const static GLfloat vertices[108] = {
	//	-1.0f, 1.0f, -1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, 1.0f, -1.0f,
	//	-1.0f, 1.0f, -1.0f,

	//	-1.0f, -1.0f, 1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f, 1.0f, -1.0f,
	//	-1.0f, 1.0f, -1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f, -1.0f, 1.0f,

	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,

	//	-1.0f, -1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	1.0f, -1.0f, 1.0f,
	//	-1.0f, -1.0f, 1.0f,

	//	-1.0f, 1.0f, -1.0f,
	//	1.0f, 1.0f, -1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f, 1.0f,
	//	-1.0f, 1.0f, -1.0f,

	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f, 1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f, 1.0f,
	//	1.0f, -1.0f, 1.0f
	//};

	//glGenVertexArrays(1, &this->VAO);
	//glGenBuffers(1, &this->VBO);
	//glBindVertexArray(this->VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glBindVertexArray(0);

	////this->camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 72.0f, (float)this->WIDTH / (float)this->HEIGHT, 0.1f, 100.0f);
	//this->simpleShader = new Shader("Data/shaders/lampShader.vert", "Data/shaders/lampShader.frag");

	

	//=================================================
	this->camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 72.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	this->shader = new Shader("Data/shaders/normal.vert", "Data/shaders/normal.frag");
	this->lamp = new PointLight("Data/shaders/lampShader.vert", "Data/shaders/lampShader.frag");

	this->mouseModel = new Model("Data/models/mouse/mousino.obj");
	this->fbShader = new Shader("Data/shaders/fb.vert", "Data/shaders/fb.frag");
	lightPos = glm::vec3(0.0f, 5.0f, 0.0f);

	this->skybox = new Skybox("Data/shaders/skybox.vert", "Data/shaders/skybox.frag");

	//LampCube lamp("Data/shaders/lampShader.vert", "Data/shaders/lampShader.frag");
	//this->lamp = lamp;
	std::vector<const GLchar*> faces;
	// Loads a cubemap texture from 6 individual texture faces
	// Order should be:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	faces.push_back("Data/cubemaps/sea/sea_rt.jpg");
	faces.push_back("Data/cubemaps/sea/sea_lf.jpg");
	faces.push_back("Data/cubemaps/sea/sea_up.jpg");
	faces.push_back("Data/cubemaps/sea/sea_dn.jpg");
	faces.push_back("Data/cubemaps/sea/sea_bk.jpg");
	faces.push_back("Data/cubemaps/sea/sea_ft.jpg");
	cubemapTexture = loadCubemap(faces);
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO


	// Load and create a texture 
	////legacy

	// Load textures
	diffuseMap = Loader::loadTexture("Data/models/mouse/mouseAlbedo.png", false);
	specularMap = Loader::loadTexture("Data/models/mouse/mouseRoughness.png", false);
	normalMap = Loader::loadTexture("Data/models/mouse/mouseNormal.png", false);

	this->shader->use();
	glUniform1i(glGetUniformLocation(this->shader->m_ShaderProgram, "material.texture_diffuse1"), 0);
	glUniform1i(glGetUniformLocation(this->shader->m_ShaderProgram, "material.texture_specular1"), 1);
	glUniform1i(glGetUniformLocation(this->shader->m_ShaderProgram, "material.texture_normal1"), 2);
	this->shader->disable();
	//framebuffers
	// Framebuffers
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUniform1i(glGetUniformLocation(this->fbShader->m_ShaderProgram, "screenTexture"), 0);

	//glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

	this->lamp->position.x = lightPos.x;
	this->lamp->position.y = lightPos.y;
	this->lamp->position.z = lightPos.z;
	this->lamp->scale = glm::vec3(0.2f, 0.2f, 0.2f);
}

//void GlWidget::updateGL()
//{
//	double currentTime = timer1.elapsed();
//	this->deltaTime = (currentTime - this->lastFrame) / 1000;
//	this->lastFrame = currentTime;
//	//std::cout << this->deltaTime << std::endl;
//}

void GlWidget::paintGL()
{
	double currentTime = timer1.elapsed();
	this->deltaTime = (currentTime - this->lastFrame) / 1000;
	this->lastFrame = currentTime;
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//========================================
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthMask(GL_TRUE);


	this->lamp->draw(*this->camera);





	this->shader->use();


	this->shader->setUniform3f("viewPos", this->camera->position.x, this->camera->position.y, this->camera->position.z);
	//glUniform1f(glGetUniformLocation(shader.m_ShaderProgram, "material.shininess"), 32.0f);
	//// Directional light
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "dirLight.specular"), 0.4f, 0.4f, 0.4f);
	//// Point light 1				 shader.m_ShaderProgram
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.position"), 0, 0, 0);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.ambient"), 0.05f, 0.05f, 0.05f);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.diffuse"), 0.8f, 0.8f, 0.8f);
	//glUniform3f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.specular"), 1.0f, 1.0f, 1.0f);
	//glUniform1f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.constant"), 1.0f);
	//glUniform1f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.linear"), 0.09);
	//glUniform1f(glGetUniformLocation(shader.m_ShaderProgram, "pointLight.quadratic"), 0.032);

	this->shader->setUniformMatrix4fv("view", this->camera->getLookAtTargetViewMatrix(glm::vec3(0, 0, 0)));
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getLookAtTargetViewMatrix(glm::vec3(0,-3,-10))));

	//shader->setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);

	this->shader->setUniformMatrix4fv("projection", this->camera->getProjectionMatrix());

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, -3.0f));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	GLfloat angle = 2.f * (GLfloat)timer1.elapsed()/100;
	//std::cout << angle << std::endl;
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));



	//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	this->shader->setUniformMatrix4fv("model", model);



	//glBindVertexArray(this->VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);



	// Bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	//Bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalMap);

	//model
	this->mouseModel->draw(*this->shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->shader->disable();


	////skybox->draw(*camera, cubemapTexture);

	//framebuffer stuff
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 //Clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	this->fbShader->use();
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	this->fbShader->disable();
	//this->paintGL();
}

void GlWidget::resizeGL(int w, int h)
{
	//std::cout << w << std::endl;
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
	float a = (float)w / (float)h;
	this->camera->setAspectRatio(a);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUniform1i(glGetUniformLocation(this->fbShader->m_ShaderProgram, "screenTexture"), 0);

}
// Generates a texture that is suited for attachments to a framebuffer
GLuint GlWidget::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WIDTH, HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint GlWidget::loadTexture(GLchar* path, GLboolean alpha)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
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
GLuint GlWidget::loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

