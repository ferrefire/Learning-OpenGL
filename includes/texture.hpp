#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad.h"
#include <string>

class Texture
{
    private:
		unsigned int ID;

		std::string name;
		int width;
		int height;
		int index;
		GLenum unit;
		GLenum textureType;
		GLenum wrapMode;
		GLenum filterMode;
		GLenum dataType;
		GLenum colorChannels;

	public:
		Texture(std::string name, int index, GLenum unit, int width, int height, GLenum dataType);
		~Texture();

		void SetDimensions(int width, int height);
		void SetUnit(GLenum unit);
		void SetTextureType(GLenum textureType);
		void SetWrapMode(GLenum wrapMode);
		void SetFilterMode(GLenum filterMode);
		void SetDataType(GLenum dataType);
		void SetColorChannels(GLenum colorChannels);

		void CreateTexture();

		const std::string &Name();
		int Index();
		unsigned int TextureID();

		void BindImage(int imageIndex);
};

#endif