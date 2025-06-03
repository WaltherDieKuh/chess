#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "board.h"
#include "piece_manager.h"
#include "mouse_handler.h"

using namespace std;

// Verschiedene Testpositionen
const std::string START_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "uniform float aspectRatio;\n"
    "uniform vec2 tilePosition;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   vec3 pos = aPos * 0.25 + vec3(tilePosition, 0.0);\n"
    "   if(aspectRatio > 1.0)\n"
    "       pos.x /= aspectRatio;\n"
    "   else\n"
    "       pos.y *= aspectRatio;\n"
    "   gl_Position = vec4(pos, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 tileColor; // Basisfarbe des Feldes (weiß/schwarz, ausgewählt, gehovert)\n"
    "uniform sampler2D pieceTexture;\n"
    "uniform bool hasPiece;\n"
    "uniform bool isPossibleMoveHighlight; // True, wenn dieses Feld ein gültiges Zugziel ist\n"
    "in vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   vec4 output_color = vec4(tileColor, 1.0);\n" // Standardmäßig die Feldfarbe
    "   \n"
    "   if (hasPiece) {\n"
    "       vec4 piece_tex_color = texture(pieceTexture, TexCoord);\n"
    "       if (piece_tex_color.a > 0.1) {\n" // Wenn die Figur an diesem Texel sichtbar ist
    "           output_color = piece_tex_color;\n" // Nutze die Farbe der Figur
    "       }\n"
    "   }\n"
    "   \n"
    "   if (isPossibleMoveHighlight) {\n"
    "       float dist_to_center = distance(TexCoord, vec2(0.5, 0.5));\n"
    "       float dot_radius = 0.15; // Radius des Punktes (0.0 bis 0.5, da TexCoord von 0-1 geht)\n"
    "       if (dist_to_center < dot_radius) {\n"
    "           // Farbe des Punktes. Die OpenGL Blend-Funktion kümmert sich um die Transparenz.\n"
    "           output_color = vec4(1.0, 1.0, 1.0, 0.1);\n"
    "       }\n"
    "   }\n"
    "   \n"
    "   FragColor = output_color;\n"
    "}\0";


float vertices[] = {
    // Position        // Textur-Koordinaten
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // unten links
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // unten rechts
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // oben rechts
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // oben links
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

// Globale Variablen für Callbacks
MouseHandler* g_mouseHandler = nullptr;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (g_mouseHandler) {
        g_mouseHandler->handleMouseButton(window, button, action, mods);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_mouseHandler) {
        g_mouseHandler->handleMouseMove(window, xpos, ypos);
    }
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
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Textur-Koordinaten
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    // PieceManager erstellen und Texturen laden
    PieceManager pieceManager;
    if (!pieceManager.loadTextures("assets")) {
        std::cout << "Failed to load piece textures!" << std::endl;
        return -1;
    }

    // ChessBoard erstellen und Figuren platzieren
    ChessBoard chessBoard;
    
    // Verschiedene Positionen testen (ändern Sie hier die Position)
    chessBoard.setupFromFEN(START_POSITION, pieceManager);
    // chessBoard.setupFromFEN(SICILIAN_DEFENSE, pieceManager);
    // chessBoard.setupFromFEN(QUEENS_GAMBIT, pieceManager);
    // chessBoard.setupFromFEN(ENDGAME_POSITION, pieceManager);
    
    cout << "Current FEN: " << chessBoard.getCurrentFEN() << endl;
    
    // Enable blending für Transparenz
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Mouse Handler erstellen
    MouseHandler mouseHandler(&chessBoard, &pieceManager);
    g_mouseHandler = &mouseHandler;
    
    // Mouse Callbacks registrieren
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    while(!glfwWindowShouldClose(window)) {
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

        // Schachbrett mit Mouse Handler zeichnen
        chessBoard.drawBoard(shaderProgram, VAO, mouseHandler);  // Correct call

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