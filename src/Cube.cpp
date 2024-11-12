#include "Cube.h"

float Vertices[] = {
    // Positions          // Colors
   -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // Bottom-left-back
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // Bottom-right-back
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // Top-right-back
   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // Top-left-back
   -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // Bottom-left-front
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // Bottom-right-front
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // Top-right-front
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f   // Top-left-front
};

// Each face of the cube is made of two triangles
unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,  // Back face
    4, 5, 6, 6, 7, 4,  // Front face
    4, 5, 1, 1, 0, 4,  // Bottom face
    7, 6, 2, 2, 3, 7,  // Top face
    4, 7, 3, 3, 0, 4,  // Left face
    5, 6, 2, 2, 1, 5   // Right face
};

Cube::Cube() : Mesh(Vertices, sizeof(Vertices), indices, sizeof(indices)) {}
