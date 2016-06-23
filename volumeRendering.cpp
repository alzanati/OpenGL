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
GLuint mu3DTex;

int m_volumeDepth = 256;
int m_volumeWidth = 256;
int m_volumeHeight = 256;

float angle = 0.f;

#define MAP_2DTEXT( TexIndex ) \
    glTexCoord2f(0.0f, 0.0f);  \
    glVertex3f(-dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 0.0f); \
    glVertex3f(dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 1.0f); \
    glVertex3f(dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(0.0f, 1.0f); \
    glVertex3f(-dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);

#define MAP_3DTEXT( TexIndex ) \
    glTexCoord3f(0.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(-dViewPortSize,-dViewPortSize,TexIndex);\
    glTexCoord3f(1.0f, 0.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(dViewPortSize,-dViewPortSize,TexIndex);\
    glTexCoord3f(1.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(dViewPortSize,dViewPortSize,TexIndex);\
    glTexCoord3f(0.0f, 1.0f, ((float)TexIndex+1.0f)/2.0f);  \
    glVertex3f(-dViewPortSize,dViewPortSize,TexIndex);

bool initGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.f);
    glDepthFunc(GL_LEQUAL);

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

    //Set the orthographic projection.
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

bool initTextures2D( char const* volumePath )
{
    m_puTextureIDs = new GLuint[ m_volumeDepth ];
    FILE *file;

    file = fopen( volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;

    // Holds the luminance buffer
    char* chBuffer = new char[ m_volumeWidth * m_volumeHeight ];
    char* chRGBABuffer = new char[ m_volumeWidth * m_volumeHeight * 4 ];

    glGenTextures( m_volumeDepth, m_puTextureIDs );

    // Read each frames and construct the texture
    for( int nIndx = 0; nIndx < m_volumeDepth; ++nIndx )
    {
        // Read the frame
        fread(chBuffer, m_volumeWidth * m_volumeHeight, 1, file);

        // Set the properties of the texture.
        glBindTexture( GL_TEXTURE_2D, m_puTextureIDs[nIndx] );
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        for( int nIndx = 0; nIndx < m_volumeWidth*m_volumeWidth; ++nIndx )
        {
            chRGBABuffer[nIndx*4] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+1] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+2] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+3] = 255;

            if( chBuffer[nIndx] < 25 )
                chRGBABuffer[nIndx*4+3] = 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_volumeWidth, m_volumeHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid *) chRGBABuffer );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    delete[] chBuffer;
    fclose(file);
    return true;
}

bool initTextures3D( char const* volumePath )
{
    char *chBuffer, *chRGBABuffer;
    FILE *file;

    chBuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth ];
    chRGBABuffer = new char[ m_volumeWidth * m_volumeHeight * m_volumeDepth * 4 ];

    file = fopen(volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;
    fread(chBuffer, m_volumeWidth * m_volumeHeight * m_volumeDepth, 1, file);

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
    glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA,
                  m_volumeWidth, m_volumeHeight, m_volumeDepth, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid *) chRGBABuffer );
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
    float fFrameCount = m_volumeDepth;
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0.03f );

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();

    // Translate and make 0.5f as the center
    // (texture co ordinate is from 0 to 1.so center of rotation has to be 0.5f)
    //glScalef(1.5f, 1.5f, 1.5f);
    glTranslatef( 0.5f, 0.5f, 0.5f );
    glRotated( 90, 0, 1, 0 );
    glRotated( angle, 1, 0, 0 );
    glRotated( -90, 0, 0, 1 );
    glTranslatef( -0.5f,-0.5f, -0.5f );

    glEnable(GL_TEXTURE_3D);
    glBindTexture( GL_TEXTURE_3D,  mu3DTex );

    for ( float fIndx = -1.0f; fIndx <= 1.0f; fIndx+=0.01f )
     {
        glBegin(GL_QUADS);
            MAP_3DTEXT( fIndx );
        glEnd();
     }
    glutSwapBuffers();
    angle += 0.3f;
}

int main( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitWindowSize( WIDTH, HEIGHT );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "Yalla Render" );
    glutReshapeFunc( reshape );
    glutTimerFunc(0, Timer, 0);
    initGL();
    initTextures3D( "/home/prof/volumeRendering/skull.raw" );
    glutDisplayFunc( display );
    glutMainLoop();

    return EXIT_SUCCESS;
}
