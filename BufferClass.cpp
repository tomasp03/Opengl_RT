#include "BufferClass.h"

void BufferClass::Delete()
{
	glDeleteBuffers(1, &mID);
}

