#pragma once
#include<GLAD/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

class BufferClass
{
protected:
	GLuint mID;
	GLsizeiptr mSize;
public:
	void Delete();
};

