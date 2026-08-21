#include "SceneTest2.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <iostream>

static constexpr float FOG_START = 8.0f;  // distance from camera where fog begins blending in
static constexpr float FOG_END   = 30.0f; // distance from camera where geometry is fully fogged out

void SceneTest2::init() {
    shaderProgram = _sceneManager->shaders().getOrCreate("lit", createLitShaderProgram);
    mesh   = loadOBJ("assets/meshes/lost_empire.obj");
    albedo = loadSolidColorTexture(205, 235, 205);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);

    // Fog color matches the clear color so distant geometry fades into the
    // background instead of showing a hard fog "wall".
    setFog(shaderProgram, { Vec3(0.08f, 0.08f, 0.08f), FOG_START, FOG_END }, true);

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    InputManager::setCursorCaptured(true);

    camera.moveSpeed = 10.0f; //make move fasterer
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
    Mat4 model = translate(Vec3(0.0f, -20.0f, 0.0f));
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
    InputManager::setCursorCaptured(false);
}
