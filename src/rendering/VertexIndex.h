#pragma once

struct VertexIndex {
    unsigned int p, n, t;

    VertexIndex(unsigned int p_, unsigned int n_, unsigned int t_)
		: p(p_), n(n_), t(t_) {}
};