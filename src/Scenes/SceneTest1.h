#pragma once
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

class SceneTest1 : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

    std::vector<PointLight> lights = { { Vec3(2.7f, 6.7f, 3.4f), Vec3(1.0f, 1.0f, 1.0f) } };

private:
    Mesh    mesh;
    Texture albedo;
    GLuint  shaderProgram = 0;
    Camera  camera;
};
