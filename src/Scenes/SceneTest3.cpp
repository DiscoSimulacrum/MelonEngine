#include "SceneTest3.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <iostream>

void SceneTest3::init() {
    shaderProgram = _sceneManager->shaders().getOrCreate("lit", createLitShaderProgram);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);
    setFog(shaderProgram, {}, false); // uFogEnabled is program state shared via ShaderCache; must reset per scene

    _parts.clear();
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_body.obj",    "",            "assets/textures/SceneTest3/cirno_body.png", 0,   0,   0   });
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_hair.obj",    "",            "assets/textures/SceneTest3/cirno_hair.png", 0,   0,   0   });
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_eyes.obj",    "",            "assets/textures/SceneTest3/cirno_eyes.png", 0,   0,   0   });
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_ice.obj",     "",            "",                                          90,  170, 220 }); // ice blue
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_bow.obj",     "",            "",                                          90,  170, 220 }); // ice blue
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_tie.obj",     "ClothesBow",  "",                                          220, 20,  20  }); // bright red
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_eyebrow.obj", "FaceEyebrow", "",                                          20,  80,  230 }); // primary blue
    _parts.push_back({ "assets/meshes/SceneTest3/cirno_eyelash.obj", "FaceEyelash", "",                                          10,  10,  10  }); // black

    _partMeshes.clear();
    _partTextures.clear();
    for (const MeshPart& part : _parts) {
        _partMeshes.push_back(loadOBJ(part.meshPath, part.groupName));
        _partTextures.push_back(part.texturePath.empty()
            ? loadSolidColorTexture(part.colorR, part.colorG, part.colorB)
            : loadTexture(part.texturePath));
    }

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);
    InputManager::setCursorCaptured(true);
}

void SceneTest3::update(float dt) {
    int fbWidth, fbHeight;
    InputManager::getFramebufferSize(fbWidth, fbHeight);
    camera.setAspect(static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    camera.update(dt);

    if (InputManager::keyJustPressed(GLFW_KEY_M)) {
        _sceneManager->requestScene("MainMenu");
    }
}

void SceneTest3::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Matrices (all parts share one model matrix: they're authored in the
    // same object space so they line up when drawn together at identity)
    Mat4 model = Mat4::identity();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"),      1, GL_FALSE, model.m);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"),       1, GL_FALSE, camera.viewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, camera.projectionMatrix());

    // Lighting
    setPointLights(shaderProgram, lights.data(), static_cast<int>(lights.size()));
    glUniform3f(glGetUniformLocation(shaderProgram, "uCameraPos"),
        camera.position.x, camera.position.y, camera.position.z);

    // Draw each part of the assembled character with its own texture/color
    for (size_t i = 0; i < _partMeshes.size(); ++i) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _partTextures[i].id);
        glBindVertexArray(_partMeshes[i].vao);
        glDrawElements(GL_TRIANGLES, _partMeshes[i].indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void SceneTest3::shutdown() {
    for (Mesh& mesh : _partMeshes) {
        freeMesh(mesh);
    }
    _partMeshes.clear();
    for (Texture& tex : _partTextures) {
        freeTexture(tex);
    }
    _partTextures.clear();
    InputManager::setCursorCaptured(false);
}
