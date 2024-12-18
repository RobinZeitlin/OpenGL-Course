#pragma once

#include "Texture.h"

#include <string>
#include <glm.hpp>

class Shader;

class Mesh {
public:
    Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);
    ~Mesh();

    void Draw(Shader* aShader);
    void apply_texture(Texture* aTexture);

    void construct_buffers(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);

    std::string meshName;

    const float* meshVertices;
    unsigned int* meshIndices;

    size_t indexCount, vertexCount;

protected:
    unsigned int VBO, VAO, EBO;
    Texture* myTexture = nullptr;
};