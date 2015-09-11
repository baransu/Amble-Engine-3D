#include "Camera.h"

Camera::Camera(glm::vec3 position, GLfloat fov, float aspectRation, float near, float far)
{
	this->position = position;
	this->fov = fov;
	this->m_AspectRatio = aspectRation;
	this->m_Near = near;
	this->m_Far = far;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	this->updateCameraVectors();
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

glm::mat4 Camera::getLookAtTargetViewMatrix(glm::vec3 targetPosition)
{
	this->forward = glm::normalize(this->position - targetPosition);
	this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	this->up = glm::normalize(glm::cross(this->right, this->forward));
	return glm::lookAt(this->position, targetPosition, this->up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(this->fov, this->m_AspectRatio, this->m_Near, this->m_Far);
}

void Camera::updateCameraVectors()
{
	glm::vec3 forward;
	forward.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
	forward.y = sin(glm::radians(this->pitch));
	forward.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
	this->forward = glm::normalize(forward);
	this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	this->up = glm::normalize(glm::cross(this->right, this->forward));

}
