#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <format>

#include "../rendering/Texture.h"

class TextureLoader {
public:
	static TextureLoader& get_instance() {
		static TextureLoader instance;
		return instance;
	}

	TextureLoader();
	~TextureLoader();

	void clean_up();

	Texture* get_texture(std::string textureName) {
		if (textures[textureName])
		{
			return textures[textureName];
		}

		Texture* texture = new Texture((filePath + textureName).c_str());
		texture->fileName = textureName;
		std::cout << std::format("From {}{}", filePath, textureName) << std::endl;

		textures[textureName] = texture;

		return texture;
	}

	std::unordered_map<std::string, Texture*> textures;
	std::string filePath = "res/textures/";
};