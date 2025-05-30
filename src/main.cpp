#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "board.h"  // ChessBoard-Header hinzufügen

using namespace std; 

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform float aspectRatio;\n"
    "uniform vec2 tilePosition;\n"
    "void main()\n"
    "{\n"
    "   vec3 pos = aPos * 0.25 + vec3(tilePosition, 0.0);\n"  // 0.25 statt 0.125
    "   if(aspectRatio > 1.0)\n"
    "       pos.x /= aspectRatio;\n"
    "   else\n"
    "       pos.y *= aspectRatio;\n"
    "   gl_Position = vec4(pos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 tileColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(tileColor, 1.0);\n"
    "}\0";


float vertices[] = {
    -0.5f, -0.5f, 0.0f,  // unten links  (Index 0)
     0.5f, -0.5f, 0.0f,  // unten rechts (Index 1)
     0.5f,  0.5f, 0.0f,  // oben rechts  (Index 2)
    -0.5f,  0.5f, 0.0f   // oben links   (Index 3)
};

unsigned int indices[] = {
    0, 1, 2,  // Erstes Dreieck
    2, 3, 0   // Zweites Dreieck
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1000, 800, "Chess Game", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glViewport(0, 0, 1000, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    int  success;
    char infoLogVertex[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogVertex << std::endl;
    }

    char infoLogFragment[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragment << std::endl;  // FRAGMENT statt VERTEX
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();  

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ChessBoard erstellen
    ChessBoard chessBoard;

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        
        // Aspect Ratio setzen
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspectRatio = (float)width / (float)height;
        int aspectLocation = glGetUniformLocation(shaderProgram, "aspectRatio");
        glUniform1f(aspectLocation, aspectRatio);

        // Schachbrett zeichnen
        chessBoard.drawBoard(shaderProgram, VAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}