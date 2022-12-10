#pragma once

#include "BufferClass.h"

class VBO : public BufferClass
{
public:
	VBO(GLfloat* array, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Data(GLfloat* array);
};

