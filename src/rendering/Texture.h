#pragma once

#include <string>

class Texture {
public:
	Texture(const char* aPath);

	std::string fileName;

	int width;
	int height;
	unsigned int textureObject;
};