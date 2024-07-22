#include "buffer.hpp"
#include "glad.h"

Buffer::Buffer(unsigned int index, unsigned int size)
{
	this->index = index;
	this->size = size;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &ID);
}

void Buffer::CreateBuffer()
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, __null, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Buffer::BindBufferBase(unsigned int bindIndex)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindIndex, ID);
}

void Buffer::BindBuffer()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void *Buffer::GetPointer()
{
	BindBuffer();
	void *ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	return ptr;
}

void Buffer::UnMapBuffer()
{
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}