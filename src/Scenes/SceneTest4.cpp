#include "SceneTest4.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <cmath>
#include <iostream>

static constexpr float PI          = 3.14159265358979f;
static constexpr float ORBIT_RADIUS = 5.0f;
static constexpr float ORBIT_HEIGHT = 3.0f;
static constexpr float ORBIT_SPEED  = 0.7f; // radians/sec

void SceneTest4::init() {
    shaderProgram = createLitShaderProgram();
    mesh   = loadOBJ("assets/meshes/utah_teapot.obj");
    albedo = loadSolidColorTexture(245, 240, 230); // pale off-white porcelain

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    InputManager::setCursorCaptured(true);
}

void SceneTest4::update(float dt) {
    int fbWidth, fbHeight;
    InputManager::getFramebufferSize(fbWidth, fbHeight);
    camera.setAspect(static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    camera.update(dt);

    // Orbit the 3 lights around the teapot, spaced 120 degrees apart.
    _orbitTime += dt;
    for (size_t i = 0; i < lights.size(); ++i) {
        float angle = _orbitTime * ORBIT_SPEED + static_cast<float>(i) * (2.0f * PI / 3.0f);
        lights[i].position = Vec3(cosf(angle) * ORBIT_RADIUS, ORBIT_HEIGHT, sinf(angle) * ORBIT_RADIUS);
    }

    if (InputManager::keyJustPressed(GLFW_KEY_M)) {
        _sceneManager->requestScene("MainMenu");
    }
}

void SceneTest4::render() {
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

void SceneTest4::shutdown() {
    freeMesh(mesh);
    freeTexture(albedo);
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
    InputManager::setCursorCaptured(false);
}
