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
  float ratio = (float) w / (float) h;
  glViewport(0, 0, WIDTH, HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.f, ratio, 0.f, 100.f);
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  gluLookAt(2.f, 2.0f, 4.5f,
            0.f, 0.f, 0.f,
            0.f ,1.f, 0.f);

  // draw axies
  glBegin(GL_LINES);
    // x-axis
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-50.f, 0.f, 0.f);
    glVertex3f(50.f, 0.f, 0.f);
    // y-axis
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, -50.f, 0.f);
    glVertex3f(0.f, 50.f, 0.f);
    // z-axis
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.f, 50.f);
    glVertex3f(0.f, 0.f, -50.f);
  glEnd();

  glBegin(GL_QUADS);
    glColor3f(1.f, 0.f, 1.f);
    glVertex3f(-1.f, -1.f, 0.f);
    glVertex3f(1.f, -1.f, 0.f);
    glVertex3f(1.f, 1.f, 0.f);
    glVertex3f(-1.f, 1.f, 0.f);
  glEnd();

  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize( WIDTH, HEIGHT );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
  glutCreateWindow(argv[1]);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  initGL();
  glutTimerFunc(0, Timer, 0);
  glutMainLoop();

  return EXIT_SUCCESS;
}
