#include "SceneMainMenu.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Shader.h"
#include <cmath>
#include <algorithm>
#include <iostream>

static constexpr float PI               = 3.14159265358979f;
static constexpr float RING_RADIUS      = 5.0f;
static constexpr float ROTATION_SECONDS = 0.25f; // "quickly rotate, stopping at the next item"

static constexpr float CAMERA_HEIGHT    = 1.3f;   // raised above the ring plane
static constexpr float CAMERA_DISTANCE  = 9.0f;   // pulled back from the ring center (also shrinks apparent model size)
static constexpr float CAMERA_PITCH_DEG = -13.0f; // tilted down so the ring's circular shape reads

void SceneMainMenu::init() {
    shaderProgram = createLitShaderProgram();
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uAlbedo"), 0);

    camera.setPose(Vec3(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE), -90.0f, CAMERA_PITCH_DEG);

    exitMesh    = loadOBJ("assets/meshes/exit.obj");
    exitTexture = loadSolidColorTexture(200, 30, 30);

    _items.clear();
    _items.push_back({ "Test Scene 1", MenuItemType::Scene, "Test1", "assets/meshes/model.obj",                 160, 170, 220,      1.5}); // smooshed disco cube
    _items.push_back({ "Test Scene 2", MenuItemType::Scene, "Test2", "assets/meshes/icosphere.obj",             205, 235, 205,      1.0}); // minecraft
    _items.push_back({ "Test Scene 3", MenuItemType::Scene, "Test3", "assets/meshes/miltPlayer_menu.obj",       255, 255, 0  ,      7.0}); // bright yellow
    _items.push_back({ "Test Scene 4", MenuItemType::Scene, "Test4", "assets/meshes/utah_teapot_lowpoly.obj",   245, 240, 230,      0.4}); // utah teapot
    _items.push_back({ "Exit",         MenuItemType::Exit,  "",      "",                                        0,   0,   0  ,      1.0}); // Exit program

    _itemMeshes.clear();
    _itemTextures.clear();
    for (const MenuEntry& item : _items) {
        _itemMeshes.push_back(item.type == MenuItemType::Exit ? Mesh{} : loadOBJ(item.meshPath));
        _itemTextures.push_back(loadSolidColorTexture(item.colorR, item.colorG, item.colorB));
    }

    _selectedIndex  = 0;
    _ringRotation   = 0.0f;
    _rotationStart  = 0.0f;
    _rotationTarget = 0.0f;
    _rotationTimer  = 0.0f;
    _isRotating     = false;

    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    InputManager::setCursorCaptured(false);

    std::cout << "[MainMenu] A/D to rotate, Enter to select (" << _items[_selectedIndex].label << ")\n";
}

void SceneMainMenu::update(float dt) {
    int fbWidth, fbHeight;
    InputManager::getFramebufferSize(fbWidth, fbHeight);
    camera.setAspect(static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    if (_isRotating) {
        _rotationTimer += dt;
        float t     = std::min(_rotationTimer / ROTATION_SECONDS, 1.0f);
        float eased = 1.0f - powf(1.0f - t, 3.0f); // ease-out cubic: fast start, snap to a stop
        _ringRotation = _rotationStart + (_rotationTarget - _rotationStart) * eased;
        if (t >= 1.0f) {
            _isRotating   = false;
            _ringRotation = _rotationTarget;
        }
        return; // ignore input while the ring is settling, like the reference inventories
    }

    if (InputManager::keyJustPressed(GLFW_KEY_D)) {
        rotateToNext(+1);
    } else if (InputManager::keyJustPressed(GLFW_KEY_A)) {
        rotateToNext(-1);
    } else if (InputManager::keyJustPressed(GLFW_KEY_ENTER)) {
        confirmSelection();
    }
}

void SceneMainMenu::rotateToNext(int direction) {
    int n = static_cast<int>(_items.size());
    _selectedIndex = ((_selectedIndex + direction) % n + n) % n;

    float anglePerItem = 2.0f * PI / static_cast<float>(n);
    _rotationStart  = _ringRotation;
    _rotationTarget = _ringRotation - direction * anglePerItem;
    _rotationTimer  = 0.0f;
    _isRotating     = true;

    std::cout << "[MainMenu] Selected: " << _items[_selectedIndex].label << "\n";
}

void SceneMainMenu::confirmSelection() {
    const MenuEntry& entry = _items[_selectedIndex];
    if (entry.type == MenuItemType::Exit) {
        glfwSetWindowShouldClose(InputManager::getWindow(), GLFW_TRUE);
    } else {
        _sceneManager->requestScene(entry.sceneName);
    }
}

void SceneMainMenu::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"),       1, GL_FALSE, camera.viewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, camera.projectionMatrix());
    setPointLights(shaderProgram, lights.data(), static_cast<int>(lights.size()));
    glUniform3f(glGetUniformLocation(shaderProgram, "uCameraPos"),
        camera.position.x, camera.position.y, camera.position.z);

    float anglePerItem = 2.0f * PI / static_cast<float>(_items.size());

    for (size_t i = 0; i < _items.size(); ++i) {
        // +PI so the selected item (canonical angle 0) faces the camera at z = +RING_RADIUS
        // instead of landing on the far side of the ring at z = -RING_RADIUS.
        float angle = static_cast<float>(i) * anglePerItem + _ringRotation + PI;
        Vec3  pos(RING_RADIUS * sinf(angle), 0.0f, -RING_RADIUS * cosf(angle));

        bool isExit = (_items[i].type == MenuItemType::Exit);
        Mat4 model = isExit
            ? translate(pos) * rotateY(PI * 0.5f) * scale(_items[i].scale)
            : translate(pos) * scale(_items[i].scale);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModel"), 1, GL_FALSE, model.m);

        const Mesh&    mesh = isExit ? exitMesh    : _itemMeshes[i];
        const Texture& tex  = isExit ? exitTexture : _itemTextures[i];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void SceneMainMenu::shutdown() {
    for (Mesh& mesh : _itemMeshes) {
        freeMesh(mesh);
    }
    _itemMeshes.clear();
    for (Texture& tex : _itemTextures) {
        freeTexture(tex);
    }
    _itemTextures.clear();
    freeMesh(exitMesh);
    freeTexture(exitTexture);
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
}
