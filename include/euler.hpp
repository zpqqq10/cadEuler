#pragma once

#include "ds.hpp"

namespace euler {
// make vertex, face and solid
Solid *mvfs(const Point &_point, Vertex *&_vertex);
// make edge and vertex
HalfEdge *mev(Vertex *const _vertex, const Point &_point, Loop *const _loop);
// make edge and face
Loop *mef(Vertex *const _vertex0, Vertex *const _vertex1, Loop *const _loop);
// kill edge and make ring
// outer loop & inner loop
Loop *kemr(Vertex *const _vertex0, Vertex *const _vertex1, Loop *const _loop);
// kill face and make ring and hole
void kfmrh(const Loop *_oloop, Loop *const _iloop);

// sweep face
Solid *sweep(Face *_face, const glm::vec3 &direction, const GLdouble distance);
} // namespace euler