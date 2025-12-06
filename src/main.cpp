#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

// Global Declarations
float AlphaMultiplier = 0.2f;
bool PrevUp = false;
bool PrevDown = false;

void ProcessInput(GLFWwindow *window);

int main()
{
    glfwInit();

    GLFWwindow *window = glfwCreateWindow(720, 500, "ABC Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Error creating the windows.\n"
                  << "Info: \n"
                  << window << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // loads the glad which is below

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error loading the glad.\n"
                  << std::endl;
        glfwTerminate();
        return -2;
    }

    std::cout << gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) << std::endl;

    Shader OurShader("assets/shader/shader.vs", "assets/shader/shader.frag");

    // vertex data
    float vertices[] = {
        // position   //    color       // texture
        -0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        +0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        +0.5f, +0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0};

    GLuint VBO, VAO, EBO; // declaration

    glGenVertexArrays(1, &VAO);
    std::cout << VAO << std::endl;
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << VBO << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    std::cout << EBO << std::endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    GLuint tex1, tex2;
    int width, height, nrChannels;

    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);

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
        std::cout << "\nFailed to load the wall jpg.\n"
                  << data << std::endl;
        glfwTerminate();
        return -3;
    }
    stbi_image_free(data);

    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("assets/image/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "\nSuccessfully load the texture.\n"
                  << std::endl;
    }
    else
    {
        std::cout << "\nFailed to load the face png.\n"
                  << data << std::endl;
        glfwTerminate();
        return -4;
    }
    stbi_image_free(data);

    // ...
    OurShader.useID();

    glUniform1i(glGetUniformLocation(OurShader.ID, "tex1"), 0);
    glUniform1i(glGetUniformLocation(OurShader.ID, "tex2"), 1);

    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(OurShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Input Commands
        ProcessInput(window);

        // render
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rendering commands
        OurShader.useID();
        glUniform1f(glGetUniformLocation(OurShader.ID, "mul"), AlphaMultiplier); // 1f -> float is sent
        
        // ...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // poll events
        glfwPollEvents();
        // swap buffer
        glfwSwapBuffers(window);
    }
    // First Triangle
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow *window)
{
    bool CurrentUp = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    if (CurrentUp) // increase
    {
        AlphaMultiplier = std::min(AlphaMultiplier + 0.01f, 1.0f); // clamp the value
        std::cout << "\nThe value of: " << AlphaMultiplier << std::endl;
    }

    bool CurrentDown = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    if (CurrentDown) // decrease
    {
        AlphaMultiplier = std::max(AlphaMultiplier - 0.01f, 0.0f); // clamp the value
        std::cout << "\nThe value of: " << AlphaMultiplier << std::endl;
    }
}
