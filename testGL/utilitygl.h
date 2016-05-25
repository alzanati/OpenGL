#ifndef UTILITYGL_H
#define UTILITYGL_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

typedef struct
{
    GLfloat x, y, z;
    GLfloat r, g, b, a;
} Vertex;

typedef struct
{
    GLfloat x, y, z;
} Data;

class UtilityGL
{
public:
    UtilityGL();

    void drawPoint( Vertex v, GLfloat size, bool smooth );

    void drawLine( Vertex v1, Vertex v2 );

    void drawPointDemo( float begin, float end, float step, int count );

    void drawLine( Vertex v1, Vertex v2, GLfloat size );

    void drawTraingle( Vertex v1, Vertex v2, Vertex v3 );

    void drawTriangleDemo();

    void drawGrid( float height, float width, float size );

    void draw2DScatterPlot( const Data* data, int numPoints );

    void drawLineBetweenPoints( const Data* data, int numPoints );

    void drawSinDemo( float phaseShift );
};

#endif // UTILITYGL_H
