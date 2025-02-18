#pragma once

#include "Texture.h"

#include <string>
#include <glm.hpp>

class Shader;

class Gizmo {
public:
    Gizmo(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);

    Gizmo()
        : meshVertices(nullptr), meshIndices(nullptr), indexCount(0), vertexCount(0), VBO(0), VAO(0), EBO(0) {
    }

    ~Gizmo();

    void Draw(Shader* aShader);
    void apply_texture(Texture* aTexture);

    std::string meshName;

    const float* meshVertices;
    unsigned int* meshIndices;

    size_t indexCount, vertexCount;

protected:
    unsigned int VBO, VAO, EBO;
    Texture* myTexture = nullptr;
};