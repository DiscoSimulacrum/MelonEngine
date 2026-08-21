#pragma once
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

class SceneTest2 : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

    // Position is overwritten each frame to track the camera
    std::vector<PointLight> lights = { { Vec3(0.0f, 0.0f, 0.0f), Vec3(3.0f, 3.0f, 3.0f) } };

private:
    Mesh    mesh;
    Texture albedo;
    GLuint  shaderProgram = 0;
    Camera  camera;
};
