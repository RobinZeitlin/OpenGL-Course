#include "EditorGrid.h"

#include "glfw3.h"
#include "glad/glad.h"

#include "../camera/Camera.h"
#include "../shaders/Shader.h"

#include <glm.hpp>

EditorGrid::EditorGrid(const float* someVertices, size_t aVertexSize) {
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, aVertexSize, someVertices, GL_STATIC_DRAW);

    vertexCount = aVertexSize / (sizeof(float) * 3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

EditorGrid::~EditorGrid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void EditorGrid::draw(Shader* aShader, Camera* camera) {
    aShader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    aShader->SetMatrix4(projection, "projection");
    aShader->SetMatrix4(view, "view");
    aShader->SetMatrix4(model, "transform");

    glLineWidth(5.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}
