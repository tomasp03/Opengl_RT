#pragma once
#include<GLAD/glad.h>
#include<GLFW/glfw3.h>
#include"VBO.h"


class VertexArray
{
	GLuint ID;
public:
	VertexArray();
	~VertexArray();
	void LinkBuffer(VBO& buffer, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
};

