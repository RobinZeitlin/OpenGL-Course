#include "Mesh.h"

#include <glfw3.h>
#include <glad/glad.h>

#include "../shaders/Shader.h"

Mesh::Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize)
    : meshVertices(someVertices), meshIndices(someIndices), vertexCount(aVertexSize), indexCount(aIndexSize)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, aVertexSize, meshVertices, GL_STATIC_DRAW);

    EBO = 0;
    if (meshIndices && aIndexSize > 0)
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndexSize, meshIndices, GL_STATIC_DRAW);
        indexCount = aIndexSize / sizeof(unsigned int);
    }
    else {
        vertexCount = aVertexSize / (8 * sizeof(float));
    }

    // v point
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vn point
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // vt point
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader* aShader)
{
    if (myTexture != NULL) {
        glBindTexture(GL_TEXTURE_2D, myTexture->textureObject);
    }

    aShader->Use();
    glBindVertexArray(VAO);

    if (EBO == 0) {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    else {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::apply_texture(Texture* aTexture) {
    myTexture = aTexture;
}
