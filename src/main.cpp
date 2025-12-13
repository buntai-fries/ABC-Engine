#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "matrix.hpp"

#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

// Global Declarations

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 10.0f);

GLboolean FirstMouse = true;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

void ProcessInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);

GLuint width = 1920;
GLuint height = 1080;

float LastX = float(width)/2.0f;
float LastY = float(height)/2.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "ABC Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Error creating the windows.\n"
                  << "Info: \n"
                  << window << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // loads the glad which is below
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); // this provides xPos and yPos

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
        // positions         // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        +0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
        +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, +0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, +0.5f, 0.0f, 0.0f,
        +0.5f, -0.5f, +0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, +0.5f, 1.0f, 1.0f,
        +0.5f, +0.5f, +0.5f, 1.0f, 1.0f,
        -0.5f, +0.5f, +0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, +0.5f, 0.0f, 0.0f,

        -0.5f, +0.5f, +0.5f, 1.0f, 0.0f,
        -0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, +0.5f, 0.0f, 0.0f,
        -0.5f, +0.5f, +0.5f, 1.0f, 0.0f,

        +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
        +0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        +0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        +0.5f, -0.5f, +0.5f, 0.0f, 0.0f,
        +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        +0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        +0.5f, -0.5f, +0.5f, 1.0f, 0.0f,
        +0.5f, -0.5f, +0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, +0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, +

                                         -0.5f,
        +0.5f, -0.5f, 0.0f, 1.0f,
        +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
        +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,
        -0.5f, +0.5f, +0.5f, 0.0f, 0.0f,
        -0.5f, +0.5f, -0.5f, 0.0f, 1.0f};

    GLuint VBO, VAO; // declaration

    glGenVertexArrays(1, &VAO);
    std::cout << VAO << std::endl;
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << VBO << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    unsigned char *data = stbi_load("assets/image/11635.jpg", &width, &height, &nrChannels, 0);
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

    // ...
    OurShader.useID();

    glUniform1i(glGetUniformLocation(OurShader.ID, "tex1"), 0);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    glEnable(GL_DEPTH_TEST);

    // rendering loop
    while (!glfwWindowShouldClose(window))
    {
        using namespace glm;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input Commands
        ProcessInput(window);

        // render
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering commands
        OurShader.useID();

        // ...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);

        mat4 view = mat4(1.0f);
        view = lookAt(CameraPos,
                      CameraPos + CameraFront,
                      CameraUp);
        glUniformMatrix4fv(glGetUniformLocation(OurShader.ID, "view"), 1, GL_FALSE, value_ptr(view));

        mat4 projection = mat4(1.0f);
        projection = perspective(radians(50.0f), (float)width / (float)height, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(OurShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

        glBindVertexArray(VAO);
        // Spawn more cubes !!!
        for (GLuint i = 0; i < 10; i++)
        {
            mat4 model = mat4(1.0f);
            model = translate(model, vec3(cubePositions[i]));
            float angle = 0.0f;
            angle = i % 3 == 0 ? (float)glfwGetTime() * 90.0f : 25.0f * i;
            model = rotate(model, radians(angle), vec3(1.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(OurShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // poll events
        glfwPollEvents();
        // swap buffer
        glfwSwapBuffers(window);
    }
    // First Triangle
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "Exiting the application.";
        glfwTerminate();
        return;
    }

    float CameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        CameraSpeed *= 5.0f;
    }

    // Manages the Camera Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        CameraPos += CameraFront * CameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        CameraPos -= CameraFront * CameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        CameraPos += glm::normalize(CameraUp) * CameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        CameraPos -= glm::normalize(CameraUp) * CameraSpeed;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    if (FirstMouse)
    {
        LastX = xPos;
        LastY = yPos;
        FirstMouse = false;
    }

    float xOffSet = xPos - LastX;
    std::cout << "LastX: " << LastX << "\t" << "xPos: " << xPos << std::endl;
    float yOffSet = LastY - yPos;
    std::cout << "LastY: " << LastY << "\t" << "yPos: " << yPos << std::endl;

    LastX = xPos;
    LastY = yPos;

    float MouseSensitivity = 0.07f;
    xOffSet *= MouseSensitivity;
    yOffSet *= MouseSensitivity;

    yaw += xOffSet;
    pitch += yOffSet;

    // constraints the pitch value for free camera:
    pitch = std::min(89.0f, pitch);
    pitch = std::max(-89.0f, pitch);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    CameraFront = glm::normalize(direction);
}
