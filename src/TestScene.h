#pragma once
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "Math.h"
#include <glad/glad.h>

class TestScene : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

    Vec3  lightDir   = { 0.4f, 1.0f, 0.5f }; // world-space direction toward the light
    Vec3  lightColor = { 1.0f, 1.0f, 1.0f };

private:
    Mesh    mesh;
    Texture albedo;
    GLuint  shaderProgram = 0;
    Camera  camera;
};
