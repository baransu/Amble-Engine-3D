#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <SOIL.h>
#include <vector>

#include "Source\Camera.h"
#include "Source\Shader.h"
#include "Source\Model.h"
#include "Source\utils\FPS.h"
#include "Source\utils\Time.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
//fpscounter
FPS fps;
Time time;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 72.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

GLfloat radius = 10.0f;

//mouse
GLfloat lastX = WIDTH/2, lastY = HEIGHT/2;
bool firstMouse = true;

bool keys[1024];

void movement();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
GLuint loadTexture(GLchar* path, GLboolean alpha);
GLuint loadCubemap(std::vector<const GLchar*> faces);

std::vector<glm::vec3> cubes;

int main()
{

	//glfw init (and windows properties)?
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//creating window and checking if correct
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initializing glew and checking if correct
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW!" << std::endl;
		return -1;
	}

	//is vsync on/off
	glfwSwapInterval(false);

	//register key callback
	glfwSetKeyCallback(window, key_callback);
	//enable and set mouse/cursor callback
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	//scroll callback
	glfwSetScrollCallback(window, scroll_callback);

	//vieport size
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//msaa
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	//glDepthMask(GL_FALSE);
	//Shader shader("res/shaders/basicLight.vert", "res/shaders/basicLight.frag");
	Shader shader("Data/shaders/normal.vert", "Data/shaders/normal.frag");
	//Shader lampShader("res/shaders/lampShader.vert", "res/shaders/lampShader.frag");
	Shader fbShader("Data/shaders/fb.vert", "Data/shaders/fb.frag");
	Shader skyboxShader("Data/shaders/skybox.vert", "Data/shaders/skybox.frag");
	Model mouseModel("Data/models/mouse/mouselowpoly.obj");
	//legacy



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
	GLuint cubemapTexture = loadCubemap(faces);
	skyboxShader.setUniform1i("skybox", 0);
	//// Set up vertex data (and buffer(s)) and attribute pointers
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

	GLfloat skyboxVertices[] = {
		// Positions          
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

	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	
	GLuint quadVAO, quadVBO;
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
	GLuint diffuseMap = loadTexture("Data/models/mouse/mouseAlbedo.png", false);
	GLuint specularMap = loadTexture("Data/models/mouse/mouseRoughness.png", false);
	GLuint normalMap = loadTexture("Data/models/mouse/mouseNormal.png", false);

	/*shader.use();
	glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "material.texture_diffuse1"), 0);
	glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "material.texture_specular1"), 1);
	glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "material.texture_normal1"), 2);
	shader.disable();*/

	shader.use();
	glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "diffuseMap"), 0);
	glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "normalMap"), 1);
	//glUniform1i(glGetUniformLocation(shader.m_ShaderProgram, "material.texture_normal1"), 2);
	shader.disable();
	
	//framebuffers
	// Framebuffers
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
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

	glUniform1i(glGetUniformLocation(fbShader.m_ShaderProgram, "screenTexture"), 0);

	//glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

	//window loop
	while (!glfwWindowShouldClose(window))
	{
		//===================
		//      EVENTS
		//===================
		glfwPollEvents();
		movement();
		//===================
		//      UPDATE
		//===================
		fps.update();
		time.update();
		//camera
		//camera.position.x = sin(glfwGetTime()) * radius;
		//camera.position.z = cos(glfwGetTime()) * radius;
		//===================
		//      RENDER
		//===================
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthMask(GL_TRUE);
		shader.use();
		GLint viewPosLoc = glGetUniformLocation(shader.m_ShaderProgram, "viewPos");
		glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);	

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

		GLint viewLoc = glGetUniformLocation(shader.m_ShaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getLookAtTargetViewMatrix(glm::vec3(0,-3,-10))));

		shader.setUniform3f("lightPos", 0, 2, -5);

		GLint projectionLoc = glGetUniformLocation(shader.m_ShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		GLfloat angle = 20.0f;
		model = glm::rotate(model, angle * (GLfloat)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::rotate(model, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		GLint modelLoc = glGetUniformLocation(shader.m_ShaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// //Bind specular map
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);

		//model
		mouseModel.draw(shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, 0);

		shader.disable();

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		viewLoc = glGetUniformLocation(skyboxShader.m_ShaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera.getViewMatrix()))));
		projectionLoc = glGetUniformLocation(skyboxShader.m_ShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		skyboxShader.disable();

		//fb stuff
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		fbShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		fbShader.disable();


		//swap buffers
		//std::cout << fps.get() << std::endl;
		glfwSwapBuffers(window);
	}

	//valid exit (delete all allocated resources)
	//glDeleteBuffers(1, &EBO);
	glDeleteFramebuffers(1, &framebuffer);
	glfwTerminate();
	return 0;
}

void movement()
{	
	GLfloat cameraSpeed = 5.0f * time.deltaTime();

	glm::vec3 tempCamFront = glm::vec3(camera.forward.x, 0, camera.forward.z);

	if (keys[GLFW_KEY_W])
	{
		camera.position += tempCamFront * cameraSpeed;
		radius -= cameraSpeed;
		if (radius < 1.0f)
			radius = 1.0f;
	}
	if (keys[GLFW_KEY_S])
	{
		camera.position -= tempCamFront * cameraSpeed;
		radius += cameraSpeed;
		if (radius > 20.0f)
			radius = 20.0f;
	}
	if (keys[GLFW_KEY_A])
		camera.position -= glm::normalize(glm::cross(camera.forward, camera.up)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		camera.position += glm::normalize(glm::cross(camera.forward, camera.up)) * cameraSpeed;
	if (keys[GLFW_KEY_Q])
		camera.position -= glm::vec3(0.0f, cameraSpeed, 0.0f);
	if (keys[GLFW_KEY_E])
		camera.position += glm::vec3(0.0f, cameraSpeed, 0.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//looking for escape key and closing window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_F3)
	{
		std::cout << fps.get() << std::endl;
		//std::cout << time.deltaTime() << std::endl;
	}

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera.yaw += xoffset;
	camera.pitch += yoffset;
	if (camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if (camera.pitch < -89.0f)
		camera.pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera.fov >= 1.0f && camera.fov <= 72.0f)
		camera.fov -= yoffset;
	if (camera.fov <= 1.0f)
		camera.fov = 1.0f;
	if (camera.fov >= 72.0f)
		camera.fov = 72.0f;
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
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

GLuint loadTexture(GLchar* path, GLboolean alpha)
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
GLuint loadCubemap(std::vector<const GLchar*> faces)
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