#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad.h"
#include <string>

class Texture
{
    private:
		unsigned int ID;

		std::string name;
		int width = 100;
		int height = 100;
		int depth = 1;
		int index = 0;
		GLenum unit = GL_TEXTURE0;
		GLenum textureType = GL_TEXTURE_2D;
		GLenum wrapMode = GL_CLAMP_TO_EDGE;
		GLenum filterMode = GL_LINEAR;
		GLenum dataType = GL_R16_SNORM;
		GLenum colorChannels = GL_RED;
		GLenum floatType = GL_HALF_FLOAT;
		bool resizable = false;

	public:
		Texture(std::string name, int index, GLenum unit, int width, int height, GLenum dataType);
		~Texture();

		void SetDimensions(int width, int height);
		void SetDimensions(int width, int height, int depth);
		void SetUnit(GLenum unit);
		void SetTextureType(GLenum textureType);
		void SetWrapMode(GLenum wrapMode);
		void SetFilterMode(GLenum filterMode);
		void SetDataType(GLenum dataType);
		void SetColorChannels(GLenum colorChannels);
		void SetFloatType(GLenum floatType);
		void SetResizable(bool resizable);

		void CreateTexture();

		const std::string &Name();
		int Index();
		unsigned int TextureID();
		int Resolution();
		float ResolutionMultiplier();

		void BindImage(int imageIndex);
		void ClearImage();
		void Resize(int width, int height);
};

#endif