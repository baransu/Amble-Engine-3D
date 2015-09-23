#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

class Camera
{
public:
	//constructor/deconstructor
	Camera::Camera(glm::vec3 position, GLfloat fov, float aspectRation, float near, float far);	
	Camera();
	~Camera();

	//variables
	GLfloat fov = 72.0f;
	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	GLfloat roll = 0.0f;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	//functions
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getLookAtTargetViewMatrix(glm::vec3 targetPosition);
	void setAspectRatio(float aspectRatio);
	float m_Near;

private:
	//variables
	float m_AspectRatio;
	float m_Far;

	//functions
	void updateCameraVectors();
 };

