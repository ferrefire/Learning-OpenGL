#include "texture.hpp"
#include "manager.hpp"

Texture::Texture(std::string name, int index, GLenum unit, int width, int height, GLenum dataType)
{
	glGenTextures(1, &this->ID);

	this->name = name;
	this->index = index;
	this->unit = unit;
	this->width = width;
	this->height = height;
	this->dataType = dataType;

	this->textureType = GL_TEXTURE_2D;
	this->wrapMode = GL_CLAMP_TO_EDGE;
	this->filterMode = GL_LINEAR;
	this->colorChannels = GL_RED;
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->ID);
}

void Texture::SetDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Texture::SetUnit(GLenum unit)
{
	this->unit = unit;
}

void Texture::SetTextureType(GLenum textureType)
{
	this->textureType = textureType;
}

void Texture::SetWrapMode(GLenum wrapMode)
{
	this->wrapMode = wrapMode;
}

void Texture::SetFilterMode(GLenum filterMode)
{
	this->filterMode = filterMode;
}

void Texture::SetDataType(GLenum dataType)
{
	this->dataType = dataType;
}

void Texture::SetColorChannels(GLenum colorChannels)
{
	this->colorChannels = colorChannels;
}

void Texture::CreateTexture()
{
	glActiveTexture(this->unit);
	glBindTexture(this->textureType, this->ID);
	glTexParameteri(this->textureType, GL_TEXTURE_WRAP_S, this->wrapMode);
	glTexParameteri(this->textureType, GL_TEXTURE_WRAP_T, this->wrapMode);
	glTexParameteri(this->textureType, GL_TEXTURE_MAG_FILTER, this->filterMode);
	glTexParameteri(this->textureType, GL_TEXTURE_MIN_FILTER, this->filterMode);
	glTexImage2D(this->textureType, 0, this->dataType, this->width, this->height, 0, this->colorChannels, GL_FLOAT, NULL);
	glActiveTexture(0);
}

const std::string &Texture::Name()
{
	return this->name;
}

int Texture::Index()
{
	return this->index;
}

unsigned int Texture::TextureID()
{
	return this->ID;
}

void Texture::BindImage(int imageIndex)
{
	//glActiveTexture(this->unit);
	glBindImageTexture(imageIndex, this->ID, 0, GL_FALSE, 0, GL_READ_WRITE, this->dataType);
}
