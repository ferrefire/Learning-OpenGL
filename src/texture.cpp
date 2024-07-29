#include "texture.hpp"
#include "manager.hpp"

Texture::Texture(std::string name, int index, GLenum unit, int width, int height, GLenum dataType)
{
	glGenTextures(1, &ID);

	this->name = name;
	this->index = index;
	this->unit = unit;
	this->width = width;
	this->height = height;
	this->dataType = dataType;

	textureType = GL_TEXTURE_2D;
	wrapMode = GL_CLAMP_TO_EDGE;
	filterMode = GL_LINEAR;
	colorChannels = GL_RED;
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::SetDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Texture::SetDimensions(int width, int height, int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
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
	glActiveTexture(unit);
	glBindTexture(textureType, ID);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterMode);
	if (textureType == GL_TEXTURE_2D_ARRAY) glTexImage3D(textureType, 0, dataType, width, height, depth, 0, colorChannels, GL_HALF_FLOAT, NULL);
	else glTexImage2D(textureType, 0, dataType, width, height, 0, colorChannels, GL_HALF_FLOAT, NULL);
	glActiveTexture(0);
}

const std::string &Texture::Name()
{
	return this->name;
}

int Texture::Index()
{
	return index;
}

unsigned int Texture::TextureID()
{
	return ID;
}

void Texture::BindImage(int imageIndex)
{
	//glActiveTexture(this->unit);
	if (textureType == GL_TEXTURE_2D_ARRAY) glBindImageTexture(imageIndex, ID, 0, GL_TRUE, 0, GL_READ_WRITE, dataType);
	else glBindImageTexture(imageIndex, ID, 0, GL_FALSE, 0, GL_READ_WRITE, dataType);
}

int Texture::Resolution()
{
	return (width);
}

float Texture::ResolutionMultiplier()
{
	return (1.0 / float(width));
}

void Texture::ClearImage()
{
	glClearTexImage(ID, 0, colorChannels, GL_HALF_FLOAT, NULL);
}
