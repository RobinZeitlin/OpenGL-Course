#pragma once

#include "../rendering/Vertex.h"
#include "../rendering/VertexIndex.h"

#include <vector>

struct MeshInfo {
    std::vector<Vertex> vertexData;
    std::vector<VertexIndex> indices;
};