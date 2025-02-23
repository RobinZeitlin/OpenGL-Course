#pragma once

#include "Texture.h"
#include <string>
#include <glm.hpp>

class Shader;

class Mesh {
public:
    Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);

    Mesh()
        : meshVertices(nullptr), meshIndices(nullptr), indexCount(0), vertexCount(0), VBO(0), VAO(0), EBO(0) {
    }

    virtual ~Mesh();

    virtual void Draw(Shader* aShader);

    virtual void apply_texture(Texture* aTexture);

    std::string meshName;

    const float* meshVertices;
    unsigned int* meshIndices;

    size_t indexCount, vertexCount;

protected:
    unsigned int VBO, VAO, EBO;
    Texture* myTexture = nullptr;
};
