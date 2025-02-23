#pragma once

#include <glm.hpp>
#include <functional>

struct Vertex {
    float x, y, z;
    float u, v;
    float nx, ny, nz;

	// hey look, a constructor
    bool operator==(const Vertex& other) const {
        return x == other.x && y == other.y && z == other.z &&
            u == other.u && v == other.v &&
            nx == other.nx && ny == other.ny && nz == other.nz;
    }
};

// hey look a hash function
struct VertexHash {
    std::size_t operator()(const Vertex& v) const {
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z) ^
            std::hash<float>()(v.u) ^ std::hash<float>()(v.v) ^
            std::hash<float>()(v.nx) ^ std::hash<float>()(v.ny) ^ std::hash<float>()(v.nz);
    }
};
