#pragma once

class Shader;
class Camera;

class EditorGrid {
public:
	EditorGrid(const float* someVertices, size_t aVertexSize);
	~EditorGrid();

	void draw(Shader* aShader, Camera* camera);

private:
	unsigned int VBO, VAO;
	size_t vertexCount;
	Shader* gridShader;
};