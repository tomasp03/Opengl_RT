#pragma once

#include<GLAD/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<string>

#define OFF 0
#define ON 1

class WindowClass
{
	void InitializeGLFW();
	void CreateWindow();
	void InitializeVariebles(int Fwidth, int Fheight, const char* Ftitle);
	int height, width;
	const char* title;
	double crntTime = 0.0;
	double prevTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

public:
	GLFWwindow* window;
	WindowClass(int width, int height, const char* title);
	~WindowClass();
	bool ShouldClose();
	void Vsync(bool state);
	void SwapBuffers();
	void PollEvents();
	void ShowFPS();

};

