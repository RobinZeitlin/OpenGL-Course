#pragma once

#include "Texture.h"

#include <string>
#include <glm.hpp>

#include "../rendering/Vertex.h"

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
    void apply_specular(Texture* aTexture);
	void apply_albedo(Texture* aTexture);

    std::string meshName;

    const float* meshVertices;
    unsigned int* meshIndices;

    std::vector<Vertex> meshVerticesList;
    std::vector<unsigned int> meshIndiceList;

    size_t indexCount, vertexCount;

protected:
    unsigned int VBO, VAO, EBO;
    Texture* myTexture = nullptr;
    Texture* mySpecular = nullptr;
	Texture* myAlbedo = nullptr;
};
