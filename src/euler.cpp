#include "euler.hpp"
#include <iostream>

namespace euler {
// make vertex, face and solid
Solid *mvfs(const Point &_point, Vertex *&_vertex) {
  // make new instances
  Solid *solid = new Solid();
  Face *face = new Face();
  Loop *loop = new Loop();
  _vertex = new Vertex();

  // set vertex
  _vertex->vPoint = new Point(_point);

  // set pointers
  solid->FaceList.push_back(face);
  face->fSolid = solid;
  face->fOuterLoop = loop;
  loop->lFace = face;

  return solid;
}

// make edge and vertex
HalfEdge *mev(Vertex *const _vertex, const Point &_point, Loop *const _loop) {
  // create new instances
  HalfEdge *halfEdge0 = new HalfEdge();
  HalfEdge *halfEdge1 = new HalfEdge();
  Edge *edge = new Edge();
  Solid *solid = _loop->lFace->fSolid;
  // new vertex, the end vertex of the newly created edge
  Vertex *vertex = new Vertex();
  // set pointers of vertex
  vertex->vPoint = new Point(_point);
  _vertex->vHE = halfEdge0;
  vertex->vHE = halfEdge1;

  // set pointers of edges
  edge->eHE0 = halfEdge0;
  edge->eHE1 = halfEdge1;

  // set pointers of halfedges
  halfEdge0->heLoop = _loop;
  halfEdge1->heLoop = _loop;
  halfEdge0->heE = edge;
  halfEdge1->heE = edge;
  halfEdge0->startV = _vertex;
  halfEdge0->endV = vertex;
  halfEdge1->startV = vertex;
  halfEdge1->endV = _vertex;
  halfEdge0->nextHE = halfEdge1;
  halfEdge1->prevHE = halfEdge0;
  if (_loop->HalfEdgeList == nullptr) {
    // empty loop
    halfEdge0->prevHE = halfEdge1;
    halfEdge1->nextHE = halfEdge0;
    _loop->HalfEdgeList = halfEdge0;
  } else {
    // insert
    HalfEdge *end_at_v0 = _loop->findEndHE(_vertex);
    halfEdge1->nextHE = end_at_v0->nextHE;
    end_at_v0->nextHE->prevHE = halfEdge1;
    halfEdge0->prevHE = end_at_v0;
    end_at_v0->nextHE = halfEdge0;
  }

  // set pointers of solid
  solid->edgeList.push_back(edge);

  return halfEdge0;
}

// make edge and face
// _loop should be the outer loop of the face, and continue from _vertex0 to
// _vertex1
Loop *mef(Vertex *const _vertex0, Vertex *const _vertex1, Loop *const _loop) {
  // create new instances
  Loop *loop = new Loop();
  Face *face = new Face();
  HalfEdge *halfEdge0 = new HalfEdge();
  HalfEdge *halfEdge1 = new HalfEdge();
  Edge *edge = new Edge();
  // _loop->lFace->fOuterLoop == _loop here
  Solid *solid = _loop->lFace->fSolid;

  // set pointers of edges
  edge->eHE0 = halfEdge0;
  edge->eHE1 = halfEdge1;

  // set pointers of halfedges
  halfEdge0->heLoop = _loop;
  halfEdge1->heLoop = loop;
  halfEdge0->heE = edge;
  halfEdge1->heE = edge;
  halfEdge0->startV = _vertex0;
  halfEdge0->endV = _vertex1;
  halfEdge1->startV = _vertex1;
  halfEdge1->endV = _vertex0;
  halfEdge0->nextHE = halfEdge1;
  halfEdge1->prevHE = halfEdge0;
  // TODO check for nullptr
  HalfEdge *end_at_v0 = _loop->findEndHE(_vertex0); // the halfedge ends at v0
  HalfEdge *end_at_v1 = _loop->findEndHE(_vertex1); // the halfedge ends at v1
  // deal with halfedges at _vertex0
  halfEdge1->nextHE = end_at_v0->nextHE;
  end_at_v0->nextHE->prevHE = halfEdge1;
  halfEdge0->prevHE = end_at_v0;
  end_at_v0->nextHE = halfEdge0;
  // deal with halfedges at _vertex1
  halfEdge0->nextHE = end_at_v1->nextHE;
  end_at_v1->nextHE->prevHE = halfEdge0;
  halfEdge1->prevHE = end_at_v1;
  end_at_v1->nextHE = halfEdge1;

  // set pointers of loop
  loop->lFace = face;
  _loop->HalfEdgeList = halfEdge0;
  loop->HalfEdgeList = halfEdge1;

  // set pointers of face
  face->fSolid = solid;
  face->fOuterLoop = loop;
  // face->fInnerLoopList.push_back(loop);

  // set pointers of solid
  solid->FaceList.push_back(face);
  solid->edgeList.push_back(edge);

  // return the newly created loop
  return loop;
}

// kill edge and make ring
// _vertex0 at the outer loop & _vertex1 at the inner loop
Loop *kemr(Vertex *const _vertex0, Vertex *const _vertex1, Loop *const _loop) {
  // create new instances
  Loop *loop = new Loop(); // an inner loop
  Solid *solid = _loop->lFace->fSolid;
  // TODO check for nullptr
  HalfEdge *bridge0 =
      _loop->find(_vertex0, _vertex1); // the halfedge from _vertex0 to _vertex1
  HalfEdge *bridge1 = bridge0->mate(); // the halfedge from _vertex1 to _vertex0
  Edge *bridgeE = bridge0->heE;        // the edge between _vertex0 and _vertex1

  // set pointers of inner loop
  loop->lFace = _loop->lFace;
  loop->HalfEdgeList = bridge0->nextHE;
  bridge0->nextHE->prevHE = bridge1->prevHE;
  bridge1->prevHE->nextHE = bridge0->nextHE;

  // set pointers of outer loop
  _loop->HalfEdgeList = bridge1->nextHE;
  bridge0->prevHE->nextHE = bridge1->nextHE;
  bridge1->nextHE->prevHE = bridge0->prevHE;

  // set pointers of the face
  loop->lFace->fInnerLoopList.push_back(loop);

  // set pointers of solid
  solid->edgeList.remove(bridgeE);

  // kill
  delete bridgeE;

  delete bridge0;
  delete bridge1;

  return loop;
}

// kill face and make ring and hole
void kfmrh(const Loop *_oloop, Loop *const _iloop) {
  Face *outerFace =
      _oloop->lFace; // should be kept, surrounded by _oloop & _iloop
  Face *innerFace = _iloop->lFace; // this face should be killed and form a hole
  Solid *solid = outerFace->fSolid;

  // set pointers of inner loop
  _iloop->lFace = outerFace;

  // set pointers of outer face
  outerFace->fInnerLoopList.push_back(_iloop);

  // set pointers of solid
  solid->FaceList.remove(innerFace);

  // kill
  delete innerFace;
  return;
}

// sweep a single loop
void sweep_loop(Loop *_loop, const GLdouble *offset) {
  HalfEdge *tmpHE, *bridgeHE;
  Vertex *nstartV, *nendV, *initialV; // newly made startV & endV & initialV
  // sweeping
  tmpHE = _loop->HalfEdgeList->nextHE;
  Vertex *originV = _loop->HalfEdgeList->startV;
  bridgeHE = mev(tmpHE->prevHE->startV,
                 Point(tmpHE->prevHE->startV->vPoint, offset), _loop);
  initialV = nstartV = bridgeHE->endV;
  for (; tmpHE->startV != originV; nstartV = nendV, tmpHE = tmpHE->nextHE) {
    bridgeHE = mev(tmpHE->startV, Point(tmpHE->startV->vPoint, offset), _loop);
    nendV = bridgeHE->endV;
    mef(nstartV, nendV, _loop);
  }

  mef(nstartV, initialV, _loop);
}

// sweep face
Solid *sweep(Face *_face, const glm::vec3 &direction, const GLdouble distance) {

  Solid *solid = _face->fSolid;
  Loop *outer = _face->fOuterLoop;
  const GLdouble OFFSET[3] = {direction.x * distance, direction.y * distance,
                              direction.z * distance};

  // outer loop
  sweep_loop(outer, OFFSET);

  // inner loop
  for (auto &loop : _face->fInnerLoopList) {
    sweep_loop(loop, OFFSET);
  }

  return solid;
}

} // namespace euler