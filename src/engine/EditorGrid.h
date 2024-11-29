#pragma once

#include <vector>
#include <glm.hpp>

class Shader;
class Camera;

class EditorGrid {
public:
	EditorGrid(const float gridWidth, const float gridHeight);
	~EditorGrid();

	void update(float deltaTime);

	std::vector<float> get_grid_vertices(const float gridWidth, const float gridHeight);
	void update_grid(const float gridWidth, const float gridHeight);
	void draw(Shader* aShader, Camera* camera);

private:
	unsigned int VBO, VAO;
	size_t vertexCount;
	Shader* gridShader;

	glm::vec3 centerPos;
};