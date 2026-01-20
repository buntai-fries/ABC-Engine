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
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 5.0f);

GLboolean FirstMouse = true;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

GLuint width = 1920;
GLuint height = 1080;

float LastX = float(width) / 2.0f;
float LastY = float(height) / 2.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void ProcessInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
// creates the view/camera matrix
glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp);

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

    Shader lightCubeShader("assets/shader/shader.vs", "assets/shader/shader.frag");
    Shader lightShader("assets/shader/shader_l.vs", "assets/shader/shader_l.frag");

    // vertex data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
        +0.5f, -0.5f, -0.5f,  
        +0.5f, +0.5f, -0.5f,  
        +0.5f, +0.5f, -0.5f,  
        -0.5f, +0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f, +0.5f, 
        +0.5f, -0.5f, +0.5f,  
        +0.5f, +0.5f, +0.5f,  
        +0.5f, +0.5f, +0.5f,  
        -0.5f, +0.5f, +0.5f, 
        -0.5f, -0.5f, +0.5f, 

        -0.5f, +0.5f, +0.5f, 
        -0.5f, +0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, +0.5f, 
        -0.5f, +0.5f, +0.5f, 

        +0.5f, +0.5f, +0.5f,  
        +0.5f, +0.5f, -0.5f,  
        +0.5f, -0.5f, -0.5f,  
        +0.5f, -0.5f, -0.5f,  
        +0.5f, -0.5f, +0.5f,  
        +0.5f, +0.5f, +0.5f,  

        -0.5f, -0.5f, -0.5f, 
        +0.5f, -0.5f, -0.5f,  
        +0.5f, -0.5f, +0.5f,  
        +0.5f, -0.5f, +0.5f,  
        -0.5f, -0.5f, +0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, +0.5f, -0.5f, 
        +0.5f, +0.5f, -0.5f,  
        +0.5f, +0.5f, +0.5f,  
        +0.5f, +0.5f, +0.5f,  
        -0.5f, +0.5f, +0.5f, 
        -0.5f, +0.5f, -0.5f, 
    };

    GLuint VBO, VAO; // declaration

    glGenVertexArrays(1, &VAO);
    std::cout << VAO << std::endl;
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << VBO << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // ...
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
        lightShader.useID();
        lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // view
        mat4 view = mat4(1.0f);
        view = LookAt(CameraPos, CameraPos + CameraFront, CameraUp);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
        // projection
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(50.0f), (float)width / (float)height, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightShader.setMat4("model", model);
        // render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // also draw the lamp object
        lightCubeShader.useID();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // poll events
        glfwPollEvents();
        // swap buffer
        glfwSwapBuffers(window);
    }
    // First Triangle
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    // End the application
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
    // std::cout << "LastX: " << LastX << "\t" << "xPos: " << xPos << std::endl;
    float yOffSet = LastY - yPos;
    // std::cout << "LastY: " << LastY << "\t" << "yPos: " << yPos << std::endl;

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

glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 translation = glm::mat4(1.0f);

    glm::vec3 D = glm::normalize(cameraPos - cameraTarget); // Direction = D.x, D.y, D.z
    glm::vec3 R = glm::normalize(glm::cross(cameraUp, D));  // Right = R.x, R.y, R.z
    glm::vec3 U = glm::cross(D, R);                         // Up = U.x, U.y, U.z

    rotation = glm::mat4(+R.x, +U.x, +D.x, +0.0f,
                         +R.y, +U.y, +D.y, +0.0f,
                         +R.z, +U.z, +D.z, +0.0f,
                         +0.0f, +0.0f, +0.0f, +1.0f);

    translation = glm::translate(glm::mat4(+1.0f), -cameraPos);

    return rotation * translation;
}
