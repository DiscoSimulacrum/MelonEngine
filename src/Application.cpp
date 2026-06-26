#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AssetLoader.h"
#include "InputManager.h"

const int WIDTH  = 800;
const int HEIGHT = 800;

// =====================================================================
// Shaders
// =====================================================================

const char* vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 uColor;

    void main() {
        FragColor = uColor;
    }
)";

// =====================================================================
// Geometry
// =====================================================================

float vertices[] = {
    // top triangle
    -0.3f,  0.0f,
     0.3f,  0.0f,
     0.0f,  0.5f,
    // left triangle
    -0.6f, -0.5f,
     0.0f, -0.5f,
    -0.3f,  0.0f,
    // right triangle
     0.0f, -0.5f,
     0.6f, -0.5f,
     0.3f,  0.0f,
};

// =====================================================================
// State
// =====================================================================

GLuint VAO, VBO;
GLuint shaderProgram;

// =====================================================================
// Shader utils
// =====================================================================

static GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Shader compile error:\n" << log << std::endl;
    }
    return shader;
}

static GLuint createProgram(const char* vertSrc, const char* fragSrc) {
    GLuint vert = compileShader(GL_VERTEX_SHADER,   vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Shader link error:\n" << log << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

// =====================================================================
// Pipeline
// =====================================================================

static void setup() {
    shaderProgram = createProgram(vertexShaderSrc, fragmentShaderSrc);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glUniform4f(glGetUniformLocation(shaderProgram, "uColor"), 0.2f, 0.75f, 0.3f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(0);
}

static void cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

// =====================================================================
// Entry point
// =====================================================================

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Melon Engine", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);

    InputManager::initialize(window);
    setup();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        InputManager::update();
        render();
        glfwSwapBuffers(window);
    }

    cleanup();
    glfwTerminate();
    return 0;
}
