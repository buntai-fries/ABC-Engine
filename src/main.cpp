#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

#include <iostream> // debug purpose

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);

const GLuint SCR_HEI = 800;
const GLuint SCR_WID = 1500;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "GLFW initalize successfully.";

    GLFWwindow *window = glfwCreateWindow(SCR_WID, SCR_HEI, "ABC Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "\nError opening window\n"
                  << window;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "\nThe loader didn't load.\n";
        return -1;
    }

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    // BACKENDS (IMPORTANT)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Shader Class
    Shader ourShader("assets/shader/shader.vs", "assets/shader/shader.frag");

    // check the number of vertex attribute available
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

    // Vertex-data
    float vertices[] =
        {
            -0.4f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.3f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f};

    GLuint VBO, VAO; // unsigned int
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // linking vertex attribute
    // interpreting the data
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // poll events
        glfwPollEvents();

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));      // stick to left
        ImGui::SetNextWindowSize(ImVec2(300, 800)); // bigger window

        

        // Rendering commands
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.useID();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDeleteBuffers(1, &VBO);

        // UI
        ImGui::Begin("Rendering Details: ");
        ImGui::Text("Shader ID: %d", ourShader.ID);
        ImGui::Text("Max Vertex Attribute(Supported): %d", nrAttributes);
        ImGui::Text("Screen Width: %d", SCR_WID);
        ImGui::Text("Screen Width: %d", SCR_HEI);
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers
        glfwSwapBuffers(window);
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "\n_frame buffer size callback_\n";
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        std::cout << "\nManual Closing...\n";
    }
}
