#pragma once

class SceneManager;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void init()             = 0;
    virtual void update(float dt)   = 0;
    virtual void render()           = 0;
    virtual void shutdown()         = 0;

    void setSceneManager(SceneManager* mgr) { _sceneManager = mgr; }

protected:
    SceneManager* _sceneManager = nullptr;
};
