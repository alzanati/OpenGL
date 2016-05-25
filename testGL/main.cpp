#include <iostream>
#include <stdlib.h>
#include "utilitygl.h"


#define WIDTH 700
#define HEIGHT 500

void initGL( void );
void display( void );
void Timer(int value);

UtilityGL* glObject;
float phaseShift = 0.f;

int main(int argc, char** argv)
{
    glObject = new UtilityGL();
    int mode = GLUT_RGB | GLUT_DOUBLE;
    glutInitDisplayMode(mode);
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("test sin");
    initGL();
    glutDisplayFunc(display);

    Timer(0);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void initGL()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.0);
}

void Timer(int value)
{
    phaseShift += 0.6f;
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glObject->drawSinDemo( phaseShift );
    glEnd();

    glutSwapBuffers();
}
