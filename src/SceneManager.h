#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Scenes/Scene.h"
#include "ShaderCache.h"

class SceneManager {
public:
    using SceneFactory = std::function<std::unique_ptr<Scene>()>;

    void registerScene(const std::string& name, SceneFactory factory);
    void requestScene(const std::string& name);

    void update(float dt);
    void render();
    void shutdown();

    ShaderCache& shaders() { return _shaderCache; }

private:
    void applyPendingTransition();

    std::unordered_map<std::string, SceneFactory> _factories;
    std::unique_ptr<Scene> _current;
    std::string            _pendingName;
    bool                   _hasPending = false;
    ShaderCache             _shaderCache;
};
