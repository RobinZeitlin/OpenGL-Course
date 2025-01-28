#pragma once

struct Vertex {
    float x, y, z;
    float u, v;
    float nx, ny, nz;

    Vertex(float x_, float y_, float z_, float u_, float v_, float nx_, float ny_, float nz_)
        : x(x_), y(y_), z(z_), u(u_), v(v_), nx(nx_), ny(ny_), nz(nz_) {}
};