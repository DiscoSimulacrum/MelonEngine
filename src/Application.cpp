#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputManager.h"
#include "SceneManager.h"
#include "Scenes/SceneMainMenu.h"
#include "Scenes/SceneTest1.h"
#include "Scenes/SceneTest2.h"
#include "Scenes/SceneTest3.h"
#include "Scenes/SceneTest4.h"

const int WIDTH  = 1280;
const int HEIGHT = 720;

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

    // Framebuffer size in pixels can differ from WIDTH/HEIGHT (DPI scaling),
    // so query it rather than trusting the window-creation request.
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
        InputManager::setFramebufferSize(width, height);
    });

    InputManager::initialize(window);

    SceneManager sceneManager;
    sceneManager.registerScene("MainMenu", [] { return std::make_unique<SceneMainMenu>(); });
    sceneManager.registerScene("Test1",    [] { return std::make_unique<SceneTest1>(); });
    sceneManager.registerScene("Test2",    [] { return std::make_unique<SceneTest2>(); });
    sceneManager.registerScene("Test3",    [] { return std::make_unique<SceneTest3>(); });
    sceneManager.registerScene("Test4",    [] { return std::make_unique<SceneTest4>(); });
    sceneManager.requestScene("MainMenu");

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        float now = static_cast<float>(glfwGetTime());
        float dt  = now - lastTime;
        lastTime  = now;

        glfwPollEvents();
        InputManager::update();
        sceneManager.update(dt);
        sceneManager.render();
        glfwSwapBuffers(window);
    }

    sceneManager.shutdown();
    glfwTerminate();
    return 0;
}
