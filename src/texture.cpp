#include "texture.hpp"
#include "manager.hpp"

Texture::Texture(const char *path)
{
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "ERROR::TEXTURE::LOADING_FAILED path: " << path << std::endl;
		Manager::Quit(EXIT_FAILURE);
	}

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture::bindTexture(int textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}