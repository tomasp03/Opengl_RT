#include "VBO.h"

VBO::VBO(GLfloat* array, GLsizeiptr size)
{
	mSize = size;
	glGenBuffers(1, &mID);
	Bind();
	Data(array);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mID);
}

void VBO::Data(GLfloat* array)
{
	glBufferData(GL_ARRAY_BUFFER, mSize, array, GL_DYNAMIC_DRAW);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
