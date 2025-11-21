#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream> // debug purpose

void framebuffer_size_callback(GLFWwindow *, int width, int height);
void processInput(GLFWwindow *window);

const int SCR_HEI = 300;
const int SCR_WID = 500;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";

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

    // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment Shader 1
    unsigned int fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    // Shader Program; link different shader in this object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Shader Program; link different shader in this object ------ 1
    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    // check the number of vertex attribute available
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "\nMaximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Vertex Data
    float vertices00[] = {
        -0.4f, 0.5f, 0.0f,
        -0.3f, 0.1f, 0.0f,
        -0.5f, 0.1f, 0.0f
    };

    float vertices01[] = {
        0.4f, 0.5f, 0.0f,
        0.3f, 0.1f, 0.0f,
        0.5f, 0.1f, 0.0f
    };
     
    // 00
    GLuint VBO00, VAO00; // unsigned int
    // VAO
    glGenVertexArrays(1, &VAO00);
    glBindVertexArray(VAO00);

    // VBO
    glGenBuffers(1, &VBO00);
    glBindBuffer(GL_ARRAY_BUFFER, VBO00);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices00), vertices00, GL_STATIC_DRAW);

    // linking vertex attribute
    // interpreting the data
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 01
    GLuint VBO01, VAO01;
    // VAO
    glGenVertexArrays(1, &VAO01);
    glBindVertexArray(VAO01);

    // VBO
    glGenBuffers(1, &VBO01);
    glBindBuffer(GL_ARRAY_BUFFER, VBO01);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices01), vertices01, GL_STATIC_DRAW);

    // linking vertex attribute
    // interpreting the data
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    /*
    // ???
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO00);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO01);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swapping buffer and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteProgram(shaderProgram);
    // 0th
    glDeleteVertexArrays(1, &VAO00);
    glDeleteBuffers(1, &VBO00);
    // 1st
    glDeleteVertexArrays(1, &VAO01);
    glDeleteBuffers(1, &VBO01);


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
    }
}
