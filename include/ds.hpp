#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <vector>

namespace euler {
class Point;
class Vertex;
class Edge;
class HalfEdge;
class Loop;
class Face;
class Solid;
class Scene;

// a point, containing specific coordinates and attributes
class Point {
public:
  Point() : data{0, 0, 0, .5, .5, .5} {}
  Point(double _x, double _y, double _z, double _r, double _g, double _b)
      : data{_x, _y, _z, _r, _g, _b} {}
  Point(const glm::vec3 &_p, const glm::vec3 &_c)
      : data{_p.x, _p.y, _p.z, _c.r, _c.g, _c.b} {}

  Point(const glm::vec3 &_p) {
    data[0] = _p.x;
    data[1] = _p.y;
    data[2] = _p.z;
    data[3] = (_p.x + RANGE) / RANGE;
    data[4] = (_p.y + RANGE) / RANGE;
    data[5] = (_p.z + RANGE) / RANGE;
  }
  Point(const Point &_p)
      : data{_p.data[0], _p.data[1], _p.data[2],
             _p.data[3], _p.data[4], _p.data[5]} {}

  Point(const Point *_p, const GLdouble *_offset)
      : data{_p->data[0] + _offset[0],
             _p->data[1] + _offset[1],
             _p->data[2] + _offset[2],
             _p->data[3],
             _p->data[4],
             _p->data[5]} {}

  // x, y, z should be within [-5, 5]
  const GLdouble RANGE = 2 * 20.0f;
  GLdouble data[6];
};

// a vertex
class Vertex {
public:
  Vertex() : vPoint(nullptr), vHE(nullptr) {}
  // one to one
  Point *vPoint;
  // whose startV is this vertex
  HalfEdge *vHE;
};

// en edge
class Edge {
public:
  Edge() : eHE0(nullptr), eHE1(nullptr) {}
  // half edge of the edge
  HalfEdge *eHE0, *eHE1;
};

// a half edge
class HalfEdge {
public:
  HalfEdge()
      : heLoop(nullptr), prevHE(nullptr), nextHE(nullptr), heE(nullptr),
        startV(nullptr), endV(nullptr) {}
  // loop of the half edge
  Loop *heLoop;
  // necessary for maintaining circular linked list
  HalfEdge *prevHE, *nextHE;
  // edge of the half edge
  Edge *heE;
  Vertex *startV, *endV;

  HalfEdge *mate() {
    return this == this->heE->eHE0 ? this->heE->eHE1 : this->heE->eHE0;
  };
};

// a loop
class Loop {
public:
  Loop() : lFace(nullptr), HalfEdgeList(nullptr) {}
  // face of the loop
  Face *lFace;
  // half edges of the loop, circular linked list, thus HalfEdge should maintain
  // 2 pointers
  HalfEdge *HalfEdgeList;

  // return the halfedge ending at endian point
  HalfEdge *findEndHE(const Vertex *_vertex) {
    HalfEdge *temp = HalfEdgeList->nextHE;
    // that is, temp ends at _vertex
    for (; temp->nextHE->startV != _vertex; temp = temp->nextHE) {
      if (temp == HalfEdgeList) {
        return nullptr;
      }
    }
    return temp;
  }

  // return the halfedge starting from v0 and ending at v1
  HalfEdge *find(const Vertex *v0, const Vertex *v1) {
    HalfEdge *temp = HalfEdgeList->nextHE;
    for (; temp->startV != v0 || temp->endV != v1; temp = temp->nextHE) {
      if (temp == HalfEdgeList) {
        return nullptr;
      }
    }
    return temp;
  }
};

// a face
class Face {
public:
  Face() : fSolid(nullptr), fOuterLoop(nullptr), fInnerLoopList() {}
  // solid of the face
  Solid *fSolid;
  // outer loop of the face
  Loop *fOuterLoop;
  // inner loops of the face
  std::list<Loop *> fInnerLoopList;
};

// a solid
class Solid {
public:
  Solid() : FaceList(), edgeList() {}
  // face of the solid
  std::list<Face *> FaceList;
  std::list<Edge *> edgeList;
};

// a scene
class Scene {
public:
  Scene() : solids() {}
  std::list<Solid *> solids;
};

} // namespace euler