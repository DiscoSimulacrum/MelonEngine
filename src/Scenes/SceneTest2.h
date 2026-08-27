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

        const Camera* getCamera() const override { return &camera; }

        // lights[0].position is overwritten each frame to track the camera
        std::vector<PointLight> lights = {
            { Vec3(0.0f, 0.0f,  0.0f), Vec3(2.0f, 2.0f, 2.0f) },
            { Vec3(12.0f, -2.5f,  40.0f), Vec3(3.0f, 0.5f, 0.5f) },
            { Vec3(-7.0f, -3.0f,  4.0f), Vec3(0.5f, 3.0f, 0.5f) },
            { Vec3(-10.0f, 4.0f,  -50.0f), Vec3(0.5f, 0.5f, 3.0f) },
        };

    private:
        Mesh    mesh;
        Texture albedo;
        GLuint  shaderProgram = 0;
        Camera  camera;
};
