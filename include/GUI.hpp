#pragma once

#include "ds.hpp"
#include "euler.hpp"
#include "painter.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

class GUI {
public:
  static float angleX;
  static float angleY;
  static float deltaAngle;
  static float deltaMove;
  static int xOrigin;
  static int yOrigin;
  static float smoothX;
  static float smoothY;
  static float smoothFactor; // 调整平滑因子

  static euler::Scene scene;
  static void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.333, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // create solids
    scene.solids.push_back(sweep_solid());
    scene.solids.push_back(euler_solid());
  }
};

void display();

void reshape(int w, int h);

void processNormalKeys(unsigned char key, int xx, int yy);

void pressKey(int key, int xx, int yy);

void releaseKey(int key, int x, int y);

void mouseMove(int x, int y);

void mouseButton(int button, int state, int x, int y);