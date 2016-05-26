#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

void initGL()
{
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h)
{
  float aspect = (float) w / (float) h;
  glViewport(0.f, 0.f, float(WIDTH), float(HEIGHT));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.f, aspect, 0.1f, 100.f);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow(argv[1]);
  glutDisplayFunc(display);
  initGL();
  glutMainLoop();
  return EXIT_SUCCESS;
}
