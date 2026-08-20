#pragma once
#include <string>
#include <vector>
#include "Scene.h"
#include "AssetLoader.h"
#include "Camera.h"
#include "MelonMath.h"
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
    float scale = 1.0f;     // uniform scale applied to the item's mesh in the ring
};

// A Tomb-Raider/Silent-Hill-style ring of models: A/D spin the ring one slot
// at a time to the next/previous item, Enter confirms whatever's centered.
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
    GLuint  shaderProgram = 0;
    std::vector<Mesh>    _itemMeshes;   // one mesh per entry in _items, indices aligned (unused/zeroed for Exit)
    std::vector<Texture> _itemTextures; // one solid-color texture per entry in _items, indices aligned
    Mesh    exitMesh;
    Texture exitTexture;
};
