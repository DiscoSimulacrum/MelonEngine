#pragma once

class SceneManager;
class Camera;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void init()             = 0;
    virtual void update(float dt)   = 0;
    virtual void render()           = 0;
    virtual void shutdown()         = 0;

    void setSceneManager(SceneManager* mgr) { _sceneManager = mgr; }

    // Scenes with a camera override this so it can be reported (e.g. console logging);
    // scenes without one (or that don't want it reported) keep the nullptr default.
    virtual const Camera* getCamera() const { return nullptr; }

protected:
    SceneManager* _sceneManager = nullptr;
};
