#include "WindowClass.h"

WindowClass::WindowClass(int width, int height, const char* title)
{
	InitializeGLFW();
	InitializeVariebles(width, height, title);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_SAMPLES, 8);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	CreateWindow();
	gladLoadGL();
	glViewport(0, 0, width, height);
}

WindowClass::~WindowClass()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void WindowClass::InitializeGLFW()
{
	bool isGlfwInitialized = glfwInit();
	if (!isGlfwInitialized)
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
	}
}

void WindowClass::CreateWindow()
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}

void WindowClass::InitializeVariebles(int Fwidth, int Fheight, const char* Ftitle)
{
	width = Fwidth;
	height = Fheight;
	title = Ftitle;
}

bool WindowClass::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void WindowClass::Vsync(bool state)
{
	if (state == ON)
	{
		glfwSwapInterval(1);
	}
	else if (state == OFF)
	{
		glfwSwapInterval(0);
	}
	else
	{
		std::cout << "Error: Undefined Vsync state." << std::endl;
	}
}

void WindowClass::SwapBuffers()
{
	glfwSwapBuffers(window);
} 

void WindowClass::PollEvents()
{
	glfwPollEvents();
}


void WindowClass::ShowFPS()
{
	// Updates counter and times
	crntTime = glfwGetTime();
	timeDiff = crntTime - prevTime;
	counter++;

	if (timeDiff >= 1.0/5.0)
	{
		// Creates new title
		std::string FPS = std::to_string((1.0 / timeDiff) * counter);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string newTitle = FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(window, newTitle.c_str());

		// Resets times and counter
		prevTime = crntTime;
		counter = 0;
	}
}