#include "EditorGrid.h"

#include <glm.hpp>

#include "glfw3.h"
#include "glad/glad.h"

#include "../camera/Camera.h"
#include "../shaders/Shader.h"

#include "../Game.h"

EditorGrid::EditorGrid(const float gridWidth, const float gridHeight) {

    auto grid = get_grid_vertices(gridWidth, gridHeight);

    const float* someVertices = grid.data();
    size_t aVertexSize = grid.size() * sizeof(float);

    centerPos = { 0, 0, 0 };

    if (VAO == 0) {
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
}

void EditorGrid::update_grid(const float gridWidth, const float gridHeight) {
    auto grid = get_grid_vertices(gridWidth, gridHeight);

    const float* updatedVertices = grid.data();
    size_t vertexSize = grid.size() * sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, updatedVertices, GL_STATIC_DRAW);

    vertexCount = vertexSize / (sizeof(float) * 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


EditorGrid::~EditorGrid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void EditorGrid::update(float deltaTime) {
    glm::vec3 cameraPos = gameInstance->flyingCamera->position;

    if (centerPos != cameraPos) {
        centerPos = cameraPos;

        update_grid(100.0f, 100.0f);
    }
}

std::vector<float> EditorGrid::get_grid_vertices(const float gridWidth, const float gridHeight) {
    std::vector<float> vertices;
    vertices.reserve((gridWidth + gridHeight - 2) * 6);

    float xOffset = gridWidth / 2.0f - centerPos.x;
    float zOffset = gridHeight / 2.0f - centerPos.z;

    auto add_point = [&vertices](float x, float y, float z) {
        vertices.push_back(glm::floor(x));
        vertices.push_back(glm::floor(y));
        vertices.push_back(glm::floor(z));
        };

    for (int x = 1; x < gridWidth; x++) {
        add_point(x - xOffset, 0.0f, -zOffset);
        add_point(x - xOffset, 0.0f, gridWidth - zOffset);
    }

    for (int z = 1; z < gridHeight; z++) {
        add_point(-xOffset, 0.0f, z - zOffset);
        add_point(gridHeight - xOffset, 0.0f, z - zOffset);
    }

    return vertices;
}

void EditorGrid::draw(Shader* aShader, Camera* camera) {
    aShader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    aShader->SetMatrix4(projection, "projection");
    aShader->SetMatrix4(view, "view");
    aShader->SetMatrix4(model, "transform");

    glLineWidth(1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}
