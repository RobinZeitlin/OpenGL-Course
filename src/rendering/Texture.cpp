#include "Texture.h"

#include <iostream>

#include <stb_image.h>

#include <glfw3.h>
#include <glad/glad.h>

Texture::Texture(const char* aPath)
{
	int channels = 0;
	width = 0;
	height = 0;

	unsigned char* data = stbi_load(aPath, &width, &height, &channels, 0);

	glGenTextures(1, &textureObject);
	glBindTexture(GL_TEXTURE_2D, textureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Could not load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}
