 #include "SceneManager.h"
#include <iostream>

void SceneManager::registerScene(const std::string& name, SceneFactory factory) {
    _factories[name] = std::move(factory);
}

void SceneManager::requestScene(const std::string& name) {
    _pendingName = name;
    _hasPending  = true;
}

void SceneManager::applyPendingTransition() {
    if (!_hasPending) return;
    _hasPending = false;

    auto it = _factories.find(_pendingName);
    if (it == _factories.end()) {
        std::cerr << "[SceneManager] Unknown scene: " << _pendingName << "\n";
        return; // keep current scene running
    }

    if (_current) _current->shutdown();
    _current = it->second();
    _current->setSceneManager(this);
    _current->init();
}

void SceneManager::update(float dt) {
    applyPendingTransition();
    if (_current) _current->update(dt);
}

void SceneManager::render() {
    if (_current) _current->render();
}

void SceneManager::shutdown() {
    if (_current) {
        _current->shutdown();
        _current.reset();
    }
    _shaderCache.shutdown();
}
