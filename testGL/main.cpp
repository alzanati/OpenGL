#include <iostream>
#include <stdlib.h>
#include "utilitygl.h"


#define WIDTH 700
#define HEIGHT 500

void initGL( void );
void display( void );
void reshape( int w, int h );

UtilityGL* glObject;
float phaseShift = 0.f;

int main(int argc, char** argv)
{
    glObject = new UtilityGL();
    int mode = GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH;
    glutInitDisplayMode(mode);
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("test sin");
    initGL();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    //glutReshapeFunc(reshape);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void initGL()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glObject->drawSinDemo( phaseShift );
    glEnd();

    phaseShift += 0.003;
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glLoadIdentity();
    float ratio = w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.f, ratio, 0.f, 100.f);
    glMatrixMode(GL_MODELVIEW);
}
