#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

float theta = 0.f;

GLuint raw_texture_load( const char *filename, int width, int height )
{
  GLuint texture;
  unsigned char *data;
  FILE *file;

  // open texture data
  file = fopen(filename, "rb");
  if (file == NULL)
    return EXIT_FAILURE;

  // allocate buffer
  data = (unsigned char*) malloc(width * height * 4);

  // read texture data
  fread(data, width * height * 4, 1, file);
  fclose(file);

  // allocate a texture name
  glGenTextures(1, &texture);

  // select our current texture
  glBindTexture(GL_TEXTURE_2D, texture);

  // select modulate to mix texture with color for shading
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

  // when texture area is large, bilinear filter the first mipmap
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // texture should tile
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // build our texture mipmaps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );

  // free buffer
  free(data);

  return texture;
}

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

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLuint texture = raw_texture_load( "foot.raw", 256, 256 );

  // setup texture mapping
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture );

  glPushMatrix();
  glRotatef( theta, 0.0f, 0.0f, 1.0f );

  glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0);
    glVertex2d(-1.0,-1.0);

    glTexCoord2d(1.0,0.0);
    glVertex2d(+1.0,-1.0);

    glTexCoord2d(1.0,1.0);
    glVertex2d(+1.0,+1.0);

    glTexCoord2d(0.0,1.0);
    glVertex2d(-1.0,+1.0);
  glEnd();

  glPopMatrix();

  theta += 0.2;
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
  glutTimerFunc(0, Timer, 0);
  initGL();
  glutMainLoop();
  return EXIT_SUCCESS;
}
