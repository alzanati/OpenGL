#include "utilitygl.h"

UtilityGL::UtilityGL()
{

}

void UtilityGL::drawPoint( Vertex v, GLfloat size, bool smooth )
{
  glPointSize(size);
  if (smooth == true )
  {
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  glBegin(GL_POINTS);
    glColor4f(v.r, v.g, v.b, v.a);
    glVertex3f(v.x, v.y, v.z);
  glEnd();
}

void UtilityGL::drawPointDemo( float begin, float end, float step, int count )
{
  GLfloat size = 2.0f;
  count = (end - begin) / step;
  int index = 0;
  for( index = 0; index <= count; begin+=step, size+=5, index++ )
  {
    Vertex v = {begin, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    drawPoint(v, size, true);
  }
}

void UtilityGL::drawLine( Vertex v1, Vertex v2, GLfloat size )
{
  glLineWidth(size);
  glBegin( GL_LINES );
    glColor4f(v1.r, v1.g, v1.b, v1.a);
    glVertex3f(v1.x, v1.y, v1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v2.x, v2.y, v2.z);
  glEnd();
}

void UtilityGL::drawTraingle( Vertex v1, Vertex v2, Vertex v3 )
{
  glBegin(GL_TRIANGLES);
    glColor4f(v1.r, v1.g, v1.b, v1.a);
    glVertex3f(v1.x, v1.y, v1.z);
    glColor4f(v2.r, v2.g, v2.b, v2.a);
    glVertex3f(v2.x, v2.y, v2.z);
    glColor4f(v3.r, v3.g, v3.b, v3.a);
    glVertex3f(v3.x, v3.y, v3.z);
  glEnd();
}

void UtilityGL::drawTriangleDemo()
{
  Vertex v1 = {0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.6f};
  Vertex v2 = {-1.0f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.6f};
  Vertex v3 = {1.0f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.6f};
  drawTraingle(v1, v2, v3);
}

void UtilityGL::drawGrid( float height, float width, float step )
{
    // draw horizontal lines
    for( float i = -height; i <= height; i+=step )
    {
          Vertex v1 = {-width, i, 0.f, 1.f, 1.f, 1.f, 1.f};
          Vertex v2 = {width, i, 0.f, 1.f, 1.f, 1.f, 1.f};
          drawLine( v1, v2, 1.f );
    }

    // draw vertical lines
    for( float i = -width; i < width; i+=step )
    {
        Vertex v1 = {i, -height, 0.f, 1.f, 1.f, 1.f, 1.f};
        Vertex v2 = {i, height, 0.f, 1.f, 1.f, 1.f, 1.f};
        drawLine( v1, v2, 1.f );
    }

    // draw x-y axis's
    Vertex v1 = {-5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.7f};
    Vertex v2 = {5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.7f};
    Vertex v3 = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.7f};
    Vertex v4 = {0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.7f};

    drawLine(v1, v2, 3.f);
    drawLine(v3, v4, 3.f);
}


void UtilityGL::draw2DScatterPlot(const Voxel *data, int numPoints)
{
    for( int i = 0; i < numPoints; i++ )
    {
        GLfloat x = data[i].x;
        GLfloat y = data[i].y;

        Vertex v = {x, y, 0.f, 0.f, 1.0f, 1.0f, 0.5f};
        drawPoint(v, 8.f, true);
    }
}

void UtilityGL::drawLineBetweenPoints(const Voxel *data, int numPoints)
{
    for( int i = 0; i < numPoints-1; i++ )
    {
        GLfloat x1 = data[i].x;
        GLfloat y1 = data[i].y;
        GLfloat x2 = data[i + 1].x;
        GLfloat y2 = data[i + 1].y;

        Vertex v1 = {x1, y1, 0.f, 0.f, 1.0f, 1.0f, 0.5f};
        Vertex v2 = {x2, y2, 0.f, 0.f, 1.0f, 1.0f, 0.5f};

        drawLine(v1, v2, 3.f);
    }
}

void UtilityGL::drawSinDemo( float phaseShift )
{
    drawGrid(5.f, 1.f, 0.1f);
    GLfloat range = 10.0f;
    const int num_points = 200;
    float amp = 0.5f;

    Voxel *data=(Voxel*)malloc(sizeof(Voxel)*num_points);
    for(int i=0; i<num_points; i++)
    {
        data[i].x = (((GLfloat)i / num_points) * range )-(range / 2.0f);
        data[i].y = amp * cosf( data[i].x * 3.14f + phaseShift );
    }

    drawLineBetweenPoints( data, num_points );
    draw2DScatterPlot( data, num_points );
    free(data);
}
