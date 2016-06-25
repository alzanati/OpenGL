#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 680
#define HEIGHT 480

GLfloat dOrthoSize = 1.0f;
GLfloat dViewPortSize = 1.0f;

GLuint* m_puTextureIDs;
GLuint mu3DTex, mDiaplayList, mVolTexureID;

int m_volumeDepth = 256;
int m_volumeWidth = 256;
int m_volumeHeight = 256;

bool initGL()
{
    /* clear colors then enabling depth test */
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.f);
    glDepthFunc(GL_LEQUAL);

    /* check glew cabability */
    if( glewInit() != GLEW_OK )
        std::cerr << "GLEW NOT INSTALLED" << std::endl;

    if( GL_TRUE != glewGetExtension("GL_EXT_texture3D") )
    {
        std::cerr << "3D texture not supported " <<
                     __LINE__ << ":" << __FUNCTION__ << std::endl;
        return false;
    }
    else
        return true;
}

void reshape( const int t_width, const int t_height )
{
    GLdouble AspectRatio = ( GLdouble )(t_width) / ( GLdouble )(t_height );
    glViewport( 0, 0, t_width, t_height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    /* Set the orthographic projection. */
    if( t_width <= t_height )
    {
        glOrtho( -dOrthoSize, dOrthoSize, -( dOrthoSize / AspectRatio ) ,
        dOrthoSize / AspectRatio, 2.0f*-dOrthoSize, 2.0f*dOrthoSize );
    }
    else
    {
        glOrtho( -dOrthoSize * AspectRatio, dOrthoSize * AspectRatio,
        -dOrthoSize, dOrthoSize, 2.0f*-dOrthoSize, 2.0f*dOrthoSize );
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

bool initTextures3D( char const* volumePath )
{
    char *chBuffer, *chRGBABuffer;
    FILE *file;

    /* create to buffers to store data from .raw file then applying blending in another buffer */
    chBuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth ];
    chRGBABuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth * 4 ];

    file = fopen(volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;
    fread(chBuffer, m_volumeWidth * m_volumeHeight * m_volumeDepth, 1, file);

    /* spaceify texture parameters */
    glGenTextures(1, &mu3DTex );
    glBindTexture( GL_TEXTURE_3D, mu3DTex );
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int size = m_volumeDepth * m_volumeHeight * m_volumeWidth;
    for( int nIndx = 0; nIndx < size; ++nIndx )
    {
        chRGBABuffer[nIndx*4] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+1] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+2] = chBuffer[nIndx];
        chRGBABuffer[nIndx*4+3] = chBuffer[nIndx];
    }

    /* store data to bounded 3d texture */
    glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA,
                  m_volumeWidth, m_volumeHeight, m_volumeDepth, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *) chRGBABuffer );

    glBindTexture( GL_TEXTURE_3D, 0 );

    fclose(file);
    delete[] chBuffer;
    delete[] chRGBABuffer;
    return true;
}

void Timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(15, Timer, 0);
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

    /* plane equations */
    static GLdouble rightPlaneX[4] = { 1.0, 0.0, 0.0, 0.0};
    static GLdouble leftPlaneX [4] = {-1.0, 0.0, 0.0, 1.0};
    static GLdouble rightPlaneY[4] = {0.0,  1.0, 0.0, 0.0};
    static GLdouble leftPlaneY [4] = {0.0, -1.0, 0.0, 1.0};
    static GLdouble rightPlaneZ[4] = {0.0, 0.0,  1.0, 0.0};
    static GLdouble leftPlaneZ [4] = {0.0, 0.0, -1.0, 1.0};

    /* texture coordinates */
    GLfloat xCoord[] = { 1.f, 0.f, 0.f, 0.f };
    GLfloat yCoord[] = { 0.f, 1.f, 0.f, 0.f };
    GLfloat zCoord[] = { 0.f, 0.f, 1.f, 0.f };

    /* enable 3d texture */
    glEnable( GL_TEXTURE_3D );
    glBindTexture( GL_TEXTURE_3D,  mu3DTex );

    /* enable automatic texture generation, this helpful to avoid multiple using of glTexCoord*/
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    /* load projection matrix to convert from 3d to 2d screen */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    /* load projection matrix to map object to word coordinates and perform transformations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* save state and perform transformation on texture to put it in (0, 0) with clipp planes */
    glPushMatrix();

        /*around with 270 get z normal slice, around x with 90 get y normal slice*/
        glRotatef(270, 1.f, 0.f, 0.f); 
        glTranslatef(-0.5, -0.5, -0.5); // translate 0.5 to get center at 0

        /* this will generate textures in eye coordinates which help us in projection */
        glTexGenfv(GL_S, GL_EYE_PLANE, xCoord); 
        glTexGenfv(GL_T, GL_EYE_PLANE, yCoord);
        glTexGenfv(GL_R, GL_EYE_PLANE, zCoord);

        /* Define the main six clip planes to cut boundaries that don't belong to slice */
        glClipPlane(GL_CLIP_PLANE0, rightPlaneX);
        glClipPlane(GL_CLIP_PLANE1, leftPlaneX);
        glClipPlane(GL_CLIP_PLANE2, rightPlaneY);
        glClipPlane(GL_CLIP_PLANE3, leftPlaneY);
        glClipPlane(GL_CLIP_PLANE4, rightPlaneZ);
        glClipPlane(GL_CLIP_PLANE5, leftPlaneZ);
    glPopMatrix();

    /* to ensure that frame buffer does not contain redundancy of data */
    /* clip planes according to current view in projection eye */
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
    glEnable(GL_CLIP_PLANE3);
    glEnable(GL_CLIP_PLANE4);
    glEnable(GL_CLIP_PLANE5);

    /* now we have extracted slice :) */

    /* draw extracted slice to ensure correct algorithm */
    glBegin( GL_QUADS );
        glVertex3f(-dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,-dViewPortSize, 0);
        glVertex3f(dViewPortSize,dViewPortSize, 0);
        glVertex3f(-dViewPortSize,dViewPortSize, 0);
    glEnd();
    
    /* swap buffers to get update buffer to render seen */
    glutSwapBuffers();
}

void unitTest( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitWindowSize( WIDTH, HEIGHT );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "Yalla Render" );
    glutReshapeFunc( reshape );
    glutTimerFunc(0, Timer, 0);
    initGL();
    initTextures3D( "/home/prof/volumeRendering/foot.raw" );
    glutDisplayFunc( display );
    glutMainLoop();
}

int main( int argc, char** argv )
{
    /* test opengl implementations */
    unitTest(argc, argv);

    return EXIT_SUCCESS;
}
