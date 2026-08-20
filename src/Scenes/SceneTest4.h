#pragma once
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

class SceneTest4 : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

    // Red, green, blue lights orbiting the teapot; positions are updated each frame.
    std::vector<PointLight> lights = {
        { Vec3(0.0f, 2.0f, 0.0f), Vec3(3.0f, 0.0f, 0.0f) }, // red
        { Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 3.0f, 0.0f) }, // green
        { Vec3(0.0f, 2.0f, 0.0f), Vec3(0.0f, 0.0f, 3.0f) }, // blue
    };

private:
    Mesh    mesh;
    Texture albedo;
    GLuint  shaderProgram = 0;
    Camera  camera;

    float _orbitTime = 0.0f;
};
