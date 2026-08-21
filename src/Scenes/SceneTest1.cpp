#include "SceneTest1.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <iostream>

void SceneTest1::init() {
    shaderProgram         = _sceneManager->shaders().getOrCreate("lit",      createLitShaderProgram);
    emissiveShaderProgram = _sceneManager->shaders().getOrCreate("emissive", createEmissiveShaderProgram);
    mesh   = loadOBJ("assets/meshes/model.obj");
    albedo = loadTexture("assets/textures/model.png");

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);
    setFog(shaderProgram, {}, false); // uFogEnabled is program state shared via ShaderCache; must reset per scene

    glUseProgram(emissiveShaderProgram);
    glUniform1i(glGetUniformLocation(emissiveShaderProgram, "uAlbedo"), 0);

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    InputManager::setCursorCaptured(true);
}

void SceneTest1::update(float dt) {
    int fbWidth, fbHeight;
    InputManager::getFramebufferSize(fbWidth, fbHeight);
    camera.setAspect(static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    camera.update(dt);

    if (InputManager::keyJustPressed(GLFW_KEY_M)) {
        _sceneManager->requestScene("MainMenu");
    }
}

void SceneTest1::render() {
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

    // Second copy of the same mesh, offset to the side, rendered emissive
    glUseProgram(emissiveShaderProgram);

    Mat4 emissiveModel = translate(Vec3(1.5f, 0.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(emissiveShaderProgram, "uModel"),      1, GL_FALSE, emissiveModel.m);
    glUniformMatrix4fv(glGetUniformLocation(emissiveShaderProgram, "uView"),       1, GL_FALSE, camera.viewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(emissiveShaderProgram, "uProjection"), 1, GL_FALSE, camera.projectionMatrix());
    glUniform1f(glGetUniformLocation(emissiveShaderProgram, "uEmissiveIntensity"), emissiveIntensity);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void SceneTest1::shutdown() {
    freeMesh(mesh);
    freeTexture(albedo);
    InputManager::setCursorCaptured(false);
}
