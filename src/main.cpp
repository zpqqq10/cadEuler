#include "GUI.hpp"
#include "euler.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  // glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH); // 添加深度缓冲区
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

  glutInitWindowPosition(500, 500);
  glutInitWindowSize(1800, 1600);
  glutCreateWindow("CAD");

  GUI::init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);            // 添加reshape函数
  glutIdleFunc(display);               // 添加idle函数
  glutKeyboardFunc(processNormalKeys); // 添加键盘响应函数
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  glutMainLoop();
  return 0;
}