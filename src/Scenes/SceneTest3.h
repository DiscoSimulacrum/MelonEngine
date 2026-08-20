#pragma once
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

class SceneTest3 : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

    // Classic 3-point rig: bright key from the camera side, dim cool fill
    // from the opposite side, and a rim/back light behind the subject.
    std::vector<PointLight> lights = {
        { Vec3( 2.0f, 3.0f,  4.0f), Vec3(1.0f, 0.95f, 0.9f) },  // key
        { Vec3(-3.0f, 1.5f,  2.0f), Vec3(0.35f, 0.4f, 0.5f) },  // fill
        { Vec3( 0.0f, 3.0f, -3.0f), Vec3(0.6f, 0.6f, 0.7f) },   // back/rim
    };

private:
    Mesh    mesh;
    Texture albedo;
    GLuint  shaderProgram = 0;
    Camera  camera;
};
