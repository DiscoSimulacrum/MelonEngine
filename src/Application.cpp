#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputManager.h"
#include "Scene.h"
#include "TestScene.h"

const int WIDTH  = 800;
const int HEIGHT = 800;

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
    glEnable(GL_DEPTH_TEST);

    InputManager::initialize(window);

    std::unique_ptr<Scene> scene = std::make_unique<TestScene>();
    scene->init();

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        float now = static_cast<float>(glfwGetTime());
        float dt  = now - lastTime;
        lastTime  = now;

        glfwPollEvents();
        InputManager::update();
        scene->update(dt);
        scene->render();
        glfwSwapBuffers(window);
    }

    scene->shutdown();
    glfwTerminate();
    return 0;
}
