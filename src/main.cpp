#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

GLuint AlphaMultiplier = 1.0f;
void ProcessInput(GLFWwindow *window);

int main()
{
    glfwInit();

    GLFWwindow *window = glfwCreateWindow(720, 500, "ABC Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Error creating the windows" << "Info: " << window << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // loads the glad which is below

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error loading the glad." << std::endl;
        glfwTerminate();
        return -2;
    }

    std::cout << gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader OurShader("assets/shader/shader.vs", "assets/shader/shader.frag");

    // First Triangle
    // triangle vertices
    float vertices[] = {
        // position   //    color       // texture
        -0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f,
        -0.5f, +0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
        +0.5f, +0.1f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f};

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexArrayAttrib(VAO, 2);
    
    // Second Triangle
    // triangle vertices
    float vertices1[] = {
        // position   //    color
        +0.5f, +0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, +0.1f, 0.0f, 1.0f, 0.0f,
        +0.5f, +0.1f, 0.0f, 0.0f, 1.0f};

    GLuint VBO1, VAO1;

    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexArrayAttrib(VAO1, 0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexArrayAttrib(VAO1, 1);

    // texture
    GLuint tex;
    int width, height, nrChannels;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load("assets/image/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "\nSuccessfully load the texture.\n"
                  << std::endl;
    }
    else
    {
        std::cout << "\nFailed to load the texture.\n"
                  << data << std::endl;
        glfwTerminate();
        return -3;
    }
    stbi_image_free(data);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering commands
        OurShader.useID();
        // First Triangle
        glBindTexture(GL_TEXTURE_2D, tex);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Second Triangle
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // poll events
        glfwPollEvents();
        // swap buffer
        glfwSwapBuffers(window);
    }
    // First Triangle
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // 2nd Triangle
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    return 0;
}

/*
void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        if (AlphaMultiplier == 1)
        {
            AlphaMultiplier += 0.1;
        }
        else
        {
            AlphaMultiplier = 1;
        }
    }
}
*/
