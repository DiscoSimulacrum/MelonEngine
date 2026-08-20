#include "SceneTest2.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <iostream>

void SceneTest2::init() {
    shaderProgram = createLitShaderProgram();
    mesh   = loadOBJ("assets/meshes/iscv2.obj");
    albedo = loadSolidColorTexture(205, 235, 205);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    InputManager::setCursorCaptured(true);
}

void SceneTest2::update(float dt) {
    int fbWidth, fbHeight;
    InputManager::getFramebufferSize(fbWidth, fbHeight);
    camera.setAspect(static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    camera.update(dt);

    lights[0].position = camera.position; // light travels with the camera

    if (InputManager::keyJustPressed(GLFW_KEY_M)) {
        _sceneManager->requestScene("MainMenu");
    }
}

void SceneTest2::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Matrices
    Mat4 model = Mat4::identity();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"),      1, GL_FALSE, model.m);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"),       1, GL_FALSE, camera.viewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, camera.projectionMatrix());

    // Lighting
    setPointLights(shaderProgram, lights.data(), static_cast<int>(lights.size()));
    glUniform3f(glGetUniformLocation(shaderProgram, "uCameraPos"),
        camera.position.x, camera.position.y, camera.position.z);

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id);

    // Draw
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SceneTest2::shutdown() {
    freeMesh(mesh);
    freeTexture(albedo);
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
    InputManager::setCursorCaptured(false);
}
