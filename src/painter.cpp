#include "painter.hpp"

using namespace euler;

GLUtesselator *tess;

void initTess() {
  // 创建 tessellation 对象
  tess = gluNewTess();
  // 设置回调函数，这些函数将在剖分过程中被调用
  gluTessCallback(tess, GLU_TESS_BEGIN, (void (*)())tessBeginCB);
  gluTessCallback(tess, GLU_TESS_END, tessEndCB);
  // gluTessCallback(tess, GLU_TESS_ERROR, (void(CALLBACK *)())tessErrorCB);
  gluTessCallback(tess, GLU_TESS_VERTEX, (void (*)())tessVertexCB);
  // gluTessCallback(tess, GLU_TESS_ERROR, gluTessError);
}

void DrawSolid(const Solid *_solid) {
  initTess();
  // iterate drawing faces
  for (const auto &value : _solid->FaceList) {
    DrawFace(value);
  }
  gluDeleteTess(tess);
}

void DrawFace(const Face *_face) {

  gluTessBeginPolygon(tess, 0);

  // draw outter Loop
  DrawLoop(_face->fOuterLoop);
  // draw inner loops
  for (auto &loop : _face->fInnerLoopList) {
    DrawLoop(loop);
  }
  gluTessEndPolygon(tess);
}

// using only lines
// void DrawLoop(const Loop *_loop) {
//   glBegin(GL_LINE_LOOP); // Begin drawing the color cube with 6 quads
//   HalfEdge *startHE = _loop->HalfEdgeList;
//   glColor3f(startHE->startV->vPoint->data[3],
//   startHE->startV->vPoint->data[4], startHE->startV->vPoint->data[5]);
//   glVertex3f(startHE->startV->vPoint->data[0],
//   startHE->startV->vPoint->data[1], startHE->startV->vPoint->data[2]);
//   HalfEdge *he = _loop->HalfEdgeList->nextHE;
//   for (; he != nullptr && he != startHE; he = he->nextHE) {
//     glColor3f(he->startV->vPoint->data[3], he->startV->vPoint->data[4],
//     he->startV->vPoint->data[5]); glVertex3f(he->startV->vPoint->data[0],
//     he->startV->vPoint->data[1], he->startV->vPoint->data[2]);
//   }
//   glEnd();
// }

// a loop is a contour of the polygon
void DrawLoop(const Loop *_loop) {
  gluTessBeginContour(tess);
  // iterate the vertices
  HalfEdge *startHE = _loop->HalfEdgeList;
  gluTessVertex(tess, startHE->startV->vPoint->data,
                startHE->startV->vPoint->data);

  HalfEdge *he = _loop->HalfEdgeList->nextHE;
  for (; he != nullptr && he != startHE; he = he->nextHE) {
    gluTessVertex(tess, he->startV->vPoint->data, he->startV->vPoint->data);
  }
  gluTessEndContour(tess);
}

// offset: x = x + 10
Solid *sweep_solid() {

  Vertex *v0;
  Solid *solid = mvfs(Point(glm::vec3(6.f, 4.f, -2.f)), v0);
  Loop *bottom_v0123 = solid->FaceList.front()->fOuterLoop;
  HalfEdge *v01 = mev(v0, Point(glm::vec3(15.f, 4.f, -2.f)), bottom_v0123);
  Vertex *v1 = v01->endV;
  HalfEdge *v12 = mev(v1, Point(glm::vec3(15.f, -5.f, -2.f)), bottom_v0123);
  Vertex *v2 = v12->endV;
  HalfEdge *v23 = mev(v2, Point(glm::vec3(6.f, -5.f, -2.f)), bottom_v0123);
  Vertex *v3 = v23->endV;
  Loop *top_v0123 = mef(v3, v0, bottom_v0123);

  // first hole
  HalfEdge *i1_bridge =
      mev(v0, Point(glm::vec3(7.f, -2.f, -2.f)), bottom_v0123);
  Vertex *i1_v0 = i1_bridge->endV;

  HalfEdge *i1_v01 =
      mev(i1_v0, Point(glm::vec3(10.f, 1.f, -2.f)), bottom_v0123);
  Vertex *i1_v1 = i1_v01->endV;
  HalfEdge *i1_v12 =
      mev(i1_v1, Point(glm::vec3(10.f, -2.f, -2.f)), bottom_v0123);
  Vertex *i1_v2 = i1_v12->endV;
  Loop *i1_v012 = mef(i1_v0, i1_v2, bottom_v0123);

  kemr(v0, i1_v0, bottom_v0123);

  kfmrh(top_v0123, i1_v012);

  GLdouble distance = 4.0;
  glm::vec3 direction(-.2, 0., 1.);

  // second hole
  HalfEdge *i2_bridge =
      mev(v0, Point(glm::vec3(11.f, 1.f, -2.f)), bottom_v0123);
  Vertex *i2_v0 = i2_bridge->endV;

  HalfEdge *i2_v01 =
      mev(i2_v0, Point(glm::vec3(14.f, 1.f, -2.f)), bottom_v0123);
  Vertex *i2_v1 = i2_v01->endV;
  HalfEdge *i2_v12 =
      mev(i2_v1, Point(glm::vec3(11.f, -2.f, -2.f)), bottom_v0123);
  Vertex *i2_v2 = i2_v12->endV;
  Loop *i2_v012 = mef(i2_v0, i2_v2, bottom_v0123);

  kemr(v0, i2_v0, bottom_v0123);

  kfmrh(top_v0123, i2_v012);

  // called only once
  sweep(top_v0123->lFace, direction, distance);

  return solid;
}

// offset: x = x - 10
Solid *euler_solid() {

  glm::vec3 coor(-14.f, 3.f, -2.f);
  Vertex *bottom_v0;

  Solid *solid = mvfs(Point(coor), bottom_v0);
  Loop *bottom_v0123 = solid->FaceList.front()->fOuterLoop;

  HalfEdge *bottom_v01 =
      mev(bottom_v0, Point(glm::vec3(-6.f, 3.f, -2.f)), bottom_v0123);
  Vertex *bottom_v1 = bottom_v01->endV;

  HalfEdge *bottom_v12 =
      mev(bottom_v1, Point(glm::vec3(-6.f, -3.f, -2.f)), bottom_v0123);
  Vertex *bottom_v2 = bottom_v12->endV;
  HalfEdge *bottom_v23 =
      mev(bottom_v2, Point(glm::vec3(-14.f, -3.f, -2.f)), bottom_v0123);
  Vertex *bottom_v3 = bottom_v23->endV;
  // make top loop
  Loop *top_v4567 = mef(bottom_v0, bottom_v3, bottom_v0123);

  HalfEdge *side_v04 =
      mev(bottom_v0, Point(glm::vec3(-14.f, 3.f, 2.f)), top_v4567);
  Vertex *top_v4 = side_v04->endV;
  HalfEdge *side_v15 =
      mev(bottom_v1, Point(glm::vec3(-6.f, 3.f, 2.f)), top_v4567);
  Vertex *top_v5 = side_v15->endV;
  HalfEdge *side_v26 =
      mev(bottom_v2, Point(glm::vec3(-6.f, -3.f, 2.f)), top_v4567);
  Vertex *top_v6 = side_v26->endV;
  HalfEdge *side_v37 =
      mev(bottom_v3, Point(glm::vec3(-14.f, -3.f, 2.f)), top_v4567);
  Vertex *top_v7 = side_v37->endV;

  mef(top_v4, top_v5, top_v4567);
  mef(top_v5, top_v6, top_v4567);
  mef(top_v6, top_v7, top_v4567);
  mef(top_v7, top_v4, top_v4567);

  // first hole
  HalfEdge *i1_bridge =
      mev(top_v4, Point(glm::vec3(-10.f, 2.f, 2.f)), top_v4567);
  Vertex *i1_top_v4 = i1_bridge->endV;
  HalfEdge *i1_top_v45 =
      mev(i1_top_v4, Point(glm::vec3(-7.f, 1.f, 2.f)), top_v4567);
  Vertex *i1_top_v5 = i1_top_v45->endV;
  HalfEdge *i1_top_v56 =
      mev(i1_top_v5, Point(glm::vec3(-10.f, 0.f, 2.f)), top_v4567);
  Vertex *i1_top_v6 = i1_top_v56->endV;
  HalfEdge *i1_top_v67 =
      mev(i1_top_v6, Point(glm::vec3(-13.f, 1.f, 2.f)), top_v4567);
  Vertex *i1_top_v7 = i1_top_v67->endV;

  Loop *i1_top_v4567 = mef(i1_top_v4, i1_top_v7, top_v4567);

  kemr(top_v4, i1_top_v4, top_v4567);

  HalfEdge *i1_side_v40 =
      mev(i1_top_v4, Point(glm::vec3(-10.f, 2.f, -2.f)), i1_top_v4567);
  Vertex *i1_bottom_v0 = i1_side_v40->endV;
  HalfEdge *i1_side_v51 =
      mev(i1_top_v5, Point(glm::vec3(-7.f, 1.f, -2.f)), i1_top_v4567);
  Vertex *i1_bottom_v1 = i1_side_v51->endV;
  HalfEdge *i1_side_v62 =
      mev(i1_top_v6, Point(glm::vec3(-10.f, 0.f, -2.f)), i1_top_v4567);
  Vertex *i1_bottom_v2 = i1_side_v62->endV;
  HalfEdge *i1_side_v73 =
      mev(i1_top_v7, Point(glm::vec3(-13.f, 1.f, -2.f)), i1_top_v4567);
  Vertex *i1_bottom_v3 = i1_side_v73->endV;

  mef(i1_bottom_v0, i1_bottom_v1, i1_top_v4567);
  mef(i1_bottom_v1, i1_bottom_v2, i1_top_v4567);
  mef(i1_bottom_v2, i1_bottom_v3, i1_top_v4567);
  mef(i1_bottom_v3, i1_bottom_v0, i1_top_v4567);

  kfmrh(bottom_v0123, i1_top_v4567);

  // second hole
  HalfEdge *i2_bridge =
      mev(top_v4, Point(glm::vec3(-10.f, -1.f, 2.f)), top_v4567);
  Vertex *i2_top_v4 = i2_bridge->endV;
  HalfEdge *i2_top_v45 =
      mev(i2_top_v4, Point(glm::vec3(-10.f, -2.f, 2.f)), top_v4567);
  Vertex *i2_top_v5 = i2_top_v45->endV;
  HalfEdge *i2_top_v56 =
      mev(i2_top_v5, Point(glm::vec3(-8.f, -2.f, 2.f)), top_v4567);
  Vertex *i2_top_v6 = i2_top_v56->endV;
  HalfEdge *i2_top_v67 =
      mev(i2_top_v6, Point(glm::vec3(-8.f, -1.f, 2.f)), top_v4567);
  Vertex *i2_top_v7 = i2_top_v67->endV;

  Loop *i2_top_v4567 = mef(i2_top_v4, i2_top_v7, top_v4567);

  kemr(top_v4, i2_top_v4, top_v4567);

  HalfEdge *i2_side_v40 =
      mev(i2_top_v4, Point(glm::vec3(-12.f, -1.f, -2.f)), i2_top_v4567);
  Vertex *i2_bottom_v0 = i2_side_v40->endV;
  HalfEdge *i2_side_v51 =
      mev(i2_top_v5, Point(glm::vec3(-12.f, -2.f, -2.f)), i2_top_v4567);
  Vertex *i2_bottom_v1 = i2_side_v51->endV;
  HalfEdge *i2_side_v62 =
      mev(i2_top_v6, Point(glm::vec3(-10.f, -2.f, -2.f)), i2_top_v4567);
  Vertex *i2_bottom_v2 = i2_side_v62->endV;
  HalfEdge *i2_side_v73 =
      mev(i2_top_v7, Point(glm::vec3(-10.f, -1.f, -2.f)), i2_top_v4567);
  Vertex *i2_bottom_v3 = i2_side_v73->endV;

  mef(i2_bottom_v0, i2_bottom_v1, i2_top_v4567);
  mef(i2_bottom_v1, i2_bottom_v2, i2_top_v4567);
  mef(i2_bottom_v2, i2_bottom_v3, i2_top_v4567);
  mef(i2_bottom_v3, i2_bottom_v0, i2_top_v4567);

  kfmrh(bottom_v0123, i2_top_v4567);

  return solid;
}

void tessBeginCB(GLenum which) { glBegin(which); }

void tessEndCB() { glEnd(); }

void tessVertexCB(const GLvoid *data) {
  // cast back to double type
  const GLdouble *ptr = (const GLdouble *)data;

  glVertex3dv(ptr);
  glColor3dv(ptr + 3);
}

// for test
void drawCubeWithHoles() {
  // 开始多边形剖分
  gluTessBeginPolygon(tess, NULL);

  // 外部轮廓
  gluTessBeginContour(tess);

  double outerVertices[4][3] = {
      {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}};

  for (int i = 0; i < 4; ++i) {
    gluTessVertex(tess, outerVertices[i], outerVertices[i]);
  }

  gluTessEndContour(tess);

  // 内部轮廓1
  gluTessBeginContour(tess);

  double innerVertices1[4][3] = {{0.25, 0.25, 0.0},
                                 {0.75, 0.25, 0.0},
                                 {0.75, 0.75, 0.0},
                                 {0.25, 0.75, 0.0}};

  for (int i = 0; i < 4; ++i) {
    gluTessVertex(tess, innerVertices1[i], innerVertices1[i]);
  }

  gluTessEndContour(tess);

  // 内部轮廓2
  gluTessBeginContour(tess);

  double innerVertices2[4][3] = {{0.25, 0.25, 0.5},
                                 {0.75, 0.25, 0.5},
                                 {0.75, 0.75, 0.5},
                                 {0.25, 0.75, 0.5}};

  for (int i = 0; i < 4; ++i) {
    gluTessVertex(tess, innerVertices2[i], innerVertices2[i]);
  }

  gluTessEndContour(tess);

  // 结束多边形剖分
  gluTessEndPolygon(tess);
}