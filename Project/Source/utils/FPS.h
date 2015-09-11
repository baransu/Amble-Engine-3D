#pragma once

#include <iostream>
#include <GLFW\glfw3.h>

class FPS
{
public:
	FPS()
	{
		m_fps = 0;
		m_numFrames = 0;
		lastTime = glfwGetTime();
	}

	void update()
	{
		currentTime = glfwGetTime();
		m_numFrames++;
		
		if (currentTime - lastTime >= 1.0f)
		{
			m_fps = m_numFrames;
			m_numFrames = 0;
			lastTime += 1.0f;

		}
	}

	unsigned int get() const
	{
		return m_fps;
	}


private:
	unsigned int m_fps;
	unsigned int m_numFrames;
	double currentTime;
	double lastTime;
};


 //double lastTime = glfwGetTime();
 //int nbFrames = 0;

 //do{

 //    // Measure speed
 //    double currentTime = glfwGetTime();
 //    nbFrames++;
 //    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
 //        // printf and reset timer
 //        printf("%f ms/frame\n", 1000.0/double(nbFrames));
 //        nbFrames = 0;
 //        lastTime += 1.0;
 //    }
