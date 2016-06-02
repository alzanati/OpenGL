#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include <shaders.hpp>

float timed = 0.f;

int main(int argc, char** argv)
{
    // setup glfw
    if(!glfwInit())
    {
        fprintf(stderr, "Glfw not init\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =  glfwCreateWindow(680, 480, "", NULL, NULL);
    if(!window)
    {
        fprintf( stderr, "Failed to open GLFW window. If you "
                         "have an Intel GPU, they are not 3.3 compatible. Try"
                         "the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // setup glew
    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Faild to init glew\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    float square_vertices[] =
    {
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    float sColor[] =
    {
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f
    };

    float tex_coord[]
    {
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f
    };

    float triangle_vertices[] =
    {
        -1.f, 0.f,
        -0.0f, 0.f,
        -1.f, 1.f
    };

    float tColor[] =
    {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };

    GLuint program = LoadShaders("/home/prof/workspace/OpenGL/startV/headers/transform.vert",
                                 "/home/prof/workspace/OpenGL/startV/headers/texture.frag");

    const char* varying[] = {"v_position"};
    glTransformFeedbackVaryings(program, 1, varying, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(program);
    glUseProgram(program);

    GLuint vao[2];
    glGenVertexArrays(2, vao);

    GLuint square_buffer;
    GLuint triangle_buffer;
    GLuint square_color;
    GLuint triangle_color;
    GLuint feedBack_buffer;
    GLuint texture_coord;

    glBindVertexArray(vao[0]);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    GLuint elements[] =
    {
        0, 1, 2, 3
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glGenBuffers(1, &square_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &square_color);
    glBindBuffer(GL_ARRAY_BUFFER, square_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sColor), sColor, GL_STATIC_DRAW);

    glGenBuffers(1, &texture_coord);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);

    GLuint position_attrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint color_attrib = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, square_color);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint tex_attrib = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(tex_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coord);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // texture
    GLuint texture;
    glGenTextures(1, &texture);

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image("/home/prof/workspace/OpenGL/startV/camera.png",
                            &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, image );
    SOIL_free_image_data(image);

    GLuint tex_location = glGetUniformLocation(program, "tex");
    glUniform1i(tex_location, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindBuffer(GL_ARRAY_BUFFER, feedBack_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), NULL, GL_STATIC_READ);

    // start rendering
    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glFlush();
    }

    glDeleteProgram(program);
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &square_buffer);
    glDeleteBuffers(1, &square_color);

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
