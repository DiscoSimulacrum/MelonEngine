#pragma once
#include <string>
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
#include "Shader.h"
#include <glad/glad.h>

enum class MenuItemType {
    Scene,
    Exit
};

struct MenuEntry {
    std::string  label;
    MenuItemType type;
    std::string  sceneName; // only used when type == MenuItemType::Scene
    std::string  meshPath;  // only used when type == MenuItemType::Scene; Exit always uses exitMesh
    unsigned char colorR, colorG, colorB; // solid color for the item's model
    float scale = 1.0f;     // uniform scale applied to the item's mesh in the menu ring
    float verticalOffset = 0.0f; //adjsut the vertical position of the individual items in the menu ring
    MaterialType materialType    = MaterialType::Lit; // which shader this item renders with
    float        emissiveIntensity = 1.0f;            // only used when materialType == Emissive
};

// A Tomb-Raider/Silent-Hill-style ring of models: A/D spin the ring and Enter selects the item/scene

class SceneMainMenu : public Scene {
public:
    void init()           override;
    void update(float dt) override;
    void render()         override;
    void shutdown()       override;

private:
    void rotateToNext(int direction); // -1 = previous (A), +1 = next (D)
    void confirmSelection();

    std::vector<MenuEntry> _items;
    int   _selectedIndex   = 0;

    float _ringRotation    = 0.0f; // current animated angle, radians
    float _rotationStart   = 0.0f;
    float _rotationTarget  = 0.0f;
    float _rotationTimer   = 0.0f;
    bool  _isRotating      = false;

    Camera  camera;
    GLuint  shaderProgram         = 0; // "lit" program, shared via ShaderCache
    GLuint  emissiveShaderProgram = 0; // "emissive" program, shared via ShaderCache
    std::vector<Mesh>    _itemMeshes;   // one mesh per entry in _items, indices aligned (unused/zeroed for Exit)
    std::vector<Texture> _itemTextures; // one solid-color texture per entry in _items, indices aligned
    Mesh    exitMesh;
    Texture exitTexture;

    //std::vector<PointLight> _lights = { { Vec3(3.0f, 6.0f, 8.0f), Vec3(3.0f, 3.0f, 3.0f) } };
    std::vector<PointLight> lights = {
        { Vec3(3.0f, 6.0f,  8.0f), Vec3(2.0f, 2.0f, 2.0f) },  // key
        { Vec3(-5.0f, -2.0f,  9.0f), Vec3(1.0f, 1.1f, 1.2f) },  // fill
    };
};
