// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <stdio.h>
#include <GL/glew.h>
#include <SFML/Window.hpp>

// Shader macro
#define GLSL(src) "#version 150 core\n" #src

// Vertex shader
const GLchar* vertexShaderSrc = GLSL(
    in float inValue;
    out float outValue;

    void main()
    {
        outValue = sqrt( inValue );
    }
);

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    sf::Window window( sf::VideoMode(800, 600, 32), "Transform Feedback",
                       sf::Style::Titlebar | sf::Style::Close, settings );

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Compile shader
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &vertexShaderSrc, NULL);
    glCompileShader(shader);

    // Create program and specify transform feedback variables
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);

    const GLchar* feedbackVaryings[] = { "outValue" };
    glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(program);
    glUseProgram(program);

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create input VBO and vertex format
    GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof( data ), data, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(program, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Create transform feedback buffer
    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof( data ), NULL, GL_STATIC_READ);

    // Perform feedback transform
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo );

    glBeginTransformFeedback(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    glFlush();

    // Fetch and print results
    GLfloat feedback[5];
    glGetBufferSubData( GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback );

    printf("%f %f %f %f %f\n", feedback[0], feedback[1], feedback[2], feedback[3], feedback[4]);

    glDeleteProgram(program);
    glDeleteShader(shader);

    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    window.close();

    return 0;
}

//assuming that the data at hand is a 256x256x256 unsigned byte data
int XDIM=256, YDIM=256, ZDIM=256;
const int size = XDIM*YDIM*ZDIM;

bool LoadVolumeFromFile(const char* fileName)
{
   FILE *pFile = fopen(fileName,"rb");
   if(NULL == pFile) {
    return false;
   }
   GLubyte* pVolume=new GLubyte[size];
   fread(pVolume,sizeof(GLubyte),size,pFile);
   fclose(pFile);

   GLuint textureID;
   //load data into a 3D texture
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_3D, textureID);

   // set the texture parameters
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glTexImage3D(GL_TEXTURE_3D,0,GL_INTENSITY,XDIM,YDIM,ZDIM,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,pVolume);
   delete [] pVolume;
   return true;
}
