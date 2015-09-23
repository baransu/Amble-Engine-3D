#pragma once

#include <iostream>
#include <GLFW\glfw3.h>

class Time
{
public:
	Time()
	{
		lastFrame = glfwGetTime();
	}

	void update()
	{
		double currentTime = glfwGetTime();
		m_DeltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
	}

	float deltaTime()
	{
		return m_DeltaTime;
	}


private:
	double lastFrame;
	float m_DeltaTime;
};

