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
GLuint* m_puTextureIDs;

int m_uImageCount = 256;
int m_uImageWidth = 256;
int m_uImageHeight = 256;


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

bool initTextures( char const* volumePath )
{
    m_puTextureIDs = new GLuint[ m_uImageCount ];
    FILE *file;

    file = fopen( volumePath, "rb");
    if (file == NULL)
        return EXIT_FAILURE;

    // Holds the luminance buffer
    char* chBuffer = new char[ m_uImageWidth * m_uImageHeight ];
    char* chRGBABuffer = new char[ m_uImageWidth * m_uImageHeight * 4 ];

    glGenTextures( m_uImageCount, m_puTextureIDs );

    // Read each frames and construct the texture
    for( int nIndx = 0; nIndx < m_uImageCount; ++nIndx )
    {
        // Read the frame
        fread(chBuffer, m_uImageWidth * m_uImageHeight, 1, file);

        // Set the properties of the texture.
        glBindTexture( GL_TEXTURE_2D, m_puTextureIDs[nIndx] );
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        for( int nIndx = 0; nIndx < m_uImageWidth*m_uImageHeight; ++nIndx )
        {
            chRGBABuffer[nIndx*4] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+1] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+2] = chBuffer[nIndx];
            chRGBABuffer[nIndx*4+3] = 255;

            if( chBuffer[nIndx] < 20 )
                chRGBABuffer[nIndx*4+3] = 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uImageWidth, m_uImageHeight , 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,(GLvoid *) chRGBABuffer );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    delete[] chBuffer;
    fclose(file);
    return true;
}

#define MAP_2DTEXT( TexIndex ) \
    glTexCoord2f(0.0f, 0.0f);  \
    glVertex3f(-dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 0.0f); \
    glVertex3f(dOrthoSize, -dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(1.0f, 1.0f); \
    glVertex3f(dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);\
    glTexCoord2f(0.0f, 1.0f); \
    glVertex3f(-dOrthoSize, dOrthoSize,(TexIndex *2*dOrthoSize/fFrameCount)-1.0f);

void display()
{
    float fFrameCount = m_uImageCount;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0.5f );
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glRotated( 45.f, 0, 1, 0 );

    glRotated( 45.f, 1, 0, 0 );

    glRotated( -45.f, 0, 0, 1 );

    glEnable(GL_TEXTURE_2D);
    for( int nIndx=0; nIndx < m_uImageCount; ++nIndx )
    {
        glBindTexture(GL_TEXTURE_2D, m_puTextureIDs[nIndx]);
        glBegin(GL_QUADS);
            MAP_2DTEXT(nIndx);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);
    }
    glutSwapBuffers();
}

int main( int argc, char** argv )
{
    glutInit(&argc, argv);
    glutInitWindowSize( WIDTH, HEIGHT );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( "Yalla Render" );
    glutReshapeFunc( reshape );

    initGL();
    initTextures( "/home/prof/volumeRendering/skull.raw" );
    glutDisplayFunc( display );
    glutMainLoop();

    return EXIT_SUCCESS;
}
