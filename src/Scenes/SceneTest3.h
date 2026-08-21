#pragma once
#include <string>
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

struct MeshPart {
    std::string   meshPath;
    std::string   groupName;              // empty => load the whole file (see loadOBJ)
    std::string   texturePath;            // empty => use the solid color below instead
    unsigned char colorR, colorG, colorB; // only used when texturePath is empty
};

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
    std::vector<MeshPart> _parts;
    std::vector<Mesh>     _partMeshes;   // one mesh per entry in _parts, indices aligned
    std::vector<Texture>  _partTextures; // one texture per entry in _parts, indices aligned

    GLuint  shaderProgram = 0;
    Camera  camera;
};
