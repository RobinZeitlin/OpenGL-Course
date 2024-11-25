#pragma once

class Shader;

#include "Texture.h"

#include "glm.hpp"

class Mesh {
public:
	Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);
	~Mesh();

	void Draw(Shader* aShader);
	void apply_texture(Texture* aTexture);

protected:
	unsigned int VBO, VAO, EBO;
	Texture* myTexture = nullptr;
};