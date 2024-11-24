#pragma once
class Texture {
public:
	Texture(const char* aPath);

	int width;
	int height;
	unsigned int textureObject;
};