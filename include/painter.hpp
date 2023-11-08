#pragma once

#include <string>
#include <vector>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// GLFW
#include "ds.hpp"
#include "euler.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

void initTess();
void drawCubeWithHoles();
void DrawSolid(const euler::Solid *_solid);
void DrawFace(const euler::Face *_face);
void DrawLoop(const euler::Loop *_loop);

// create solids
euler::Solid *sweep_solid();
euler::Solid *euler_solid();

void tessBeginCB(GLenum which);
void tessEndCB();
void tessVertexCB(const GLvoid *data);