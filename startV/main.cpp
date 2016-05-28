#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shaders.hpp"

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

    GLFWwindow* window =  glfwCreateWindow(680, 480, "startV", NULL, NULL);
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

    GLuint program = LoadShaders("/home/mohamed/workspace/OpenGl/startV/simple.vert",
                                 "/home/mohamed/workspace/OpenGl/startV/simple.frag");
    glBindFragDataLocation(program, 0, "color_out");
    glUseProgram(program);

    GLuint vertex_buffer;
    GLuint color_buffer;
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &color_buffer);

    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    const GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        -0.5f, 0.5f, 0.f
    };
    const GLfloat colors[]=
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0, 1.0, 0.f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLint position_attrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //should bind to shader attribute

    GLint color_attrib = glGetAttribLocation(program, "color_in");
    glEnableVertexAttribArray(color_attrib);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //should bind to shader attribute

    /* test glm
    glm::mat2 mat(10.f);
    glm::vec2 myVec2 = glm::vec2(1.f, 4.f);
    glm::vec3 ve33(1.f);
    glm::vec2 tt = mat[0];
    std::cout << tt.x << std::endl;
    */

    /*
    glm::vec2 x(5.f, 4.f);
    glm::vec2 y(6.f, 5.f);
    glm::vec2 vv = x * y;
    std::cout << vv.x << std::endl;
    */

    /*
    glm::vec4 my = glm::vec4(1.f, 2.f, 3.f, 2.f);
    glm::mat4 myMat4 = glm::mat4(my, my, my, my);
    myMat4 = myMat4 * myMat4;
    std::cout << myMat4[0].y << std::endl; // first row second element
    */

    /*
    const glm::mat4 myMat4 = glm::mat4(1.f, 0.f, 0.f, 1.f,
                                       1.f, 2.f, 1.f, 1.f,
                                       1.f, 2.f, 1.f, 1.f,
                                       1.f, 2.f, 1.f, 1.f);
    glm::vec4 my = glm::vec4(1.f, 2.f, 3.f, 2.f);
    glm::vec4 mm = myMat4 * my;
    std::cout << mm.y << std::endl;
    */

    while(!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle from the 2 triangles using 6 vertices
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //clean up the memories
    glDisableVertexAttribArray(position_attrib);
    glDisableVertexAttribArray(color_attrib);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteVertexArrays(1, &vertex_array_id);
    glDeleteProgram(program);

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
