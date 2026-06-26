#pragma once
#include "Math.h"

class Camera {
public:
    Camera(float fovDeg = 45.0f, float aspect = 1.0f,
           float zNear = 0.1f,   float zFar   = 100.0f);

    void update(float dt);
    void setAspect(float aspect);

    const float* viewMatrix()       const { return _view.m; }
    const float* projectionMatrix() const { return _proj.m; }

    Vec3  position  = { 0.0f, 0.0f, 3.0f };
    float yaw       = -90.0f; // degrees; -90 faces down -Z
    float pitch     = 0.0f;   // degrees; clamped to [-89, 89]
    float moveSpeed = 3.0f;
    float lookSpeed = 0.1f;
    float fovDeg    = 45.0f;

private:
    Mat4  _view;
    Mat4  _proj;
    float _aspect;
    float _zNear;
    float _zFar;

    void rebuildView();
    void rebuildProjection();
};
