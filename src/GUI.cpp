#include "GUI.hpp"

float GUI::angleX = 0.0;
float GUI::angleY = 0.0;
float GUI::deltaAngle = 1.0;
float GUI::deltaMove = 0.0;
int GUI::xOrigin = -1;
int GUI::yOrigin = -1;
float GUI::smoothX = 0.0;
float GUI::smoothY = 0.0;
float GUI::smoothFactor = 0.1;
euler::Scene GUI::scene;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // 设置视角
  gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
  glTranslatef(0, 0, GUI::deltaMove);
  glRotatef(GUI::angleX, 1, 0, 0);
  glRotatef(GUI::angleY, 0, 1, 0);

  if (!GUI::scene.solids.empty()) {
    for (auto &solid : GUI::scene.solids) {
      DrawSolid(solid);
    }
  } else {
    std::cout << "The list is empty." << std::endl;
  }

  // glFlush();
  glutSwapBuffers();
}

void reshape(int w, int h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, .1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, int xx, int yy) {
  if (key == 27)
    exit(0);
}

void pressKey(int key, int xx, int yy) {
  switch (key) {
  case GLUT_KEY_UP:
    GUI::deltaMove += 0.5;
    break;
  case GLUT_KEY_DOWN:
    GUI::deltaMove += -0.5;
    break;
  }
}

void releaseKey(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
  case GLUT_KEY_DOWN:
    GUI::deltaMove += 0;
    break;
  }
}

void mouseMove(int x, int y) {
  if (GUI::xOrigin >= 0) {
    // 计算水平和垂直方向上的鼠标移动偏移量
    int deltaX = x - GUI::xOrigin;
    int deltaY = y - GUI::yOrigin;

    // 使用滑动平均来平滑处理鼠标移动
    GUI::smoothY =
        GUI::smoothY * (1.0 - GUI::smoothFactor) + deltaY * GUI::smoothFactor;
    GUI::smoothX =
        GUI::smoothX * (1.0 - GUI::smoothFactor) + deltaX * GUI::smoothFactor;

    // 鼠标水平移动控制左右旋转
    GUI::angleY += GUI::smoothX * 0.1;

    // 鼠标垂直移动控制上下旋转
    GUI::angleX += GUI::smoothY * 0.1;

    // 防止视角超出上下范围
    if (GUI::angleY > 360)
      GUI::angleY -= 360;
    if (GUI::angleY < -360)
      GUI::angleY += 360;

    if (GUI::angleX > 360)
      GUI::angleX -= 360;
    if (GUI::angleX < -360)
      GUI::angleX += 360;
  }
  GUI::xOrigin = x;
  GUI::yOrigin = y;
}

void mouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    GUI::xOrigin = -1;
  }
}
