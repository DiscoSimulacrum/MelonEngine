#include "Camera.h"
#include "InputManager.h"
#include <cmath>

static constexpr float PI      = 3.14159265358979f;
static constexpr float DEG2RAD = PI / 180.0f;

Camera::Camera(float fovDeg, float aspect, float zNear, float zFar)
    : fovDeg(fovDeg), _aspect(aspect), _zNear(zNear), _zFar(zFar)
{
    rebuildView();
    rebuildProjection();
}

void Camera::setAspect(float aspect) {
    _aspect = aspect;
    rebuildProjection();
}

void Camera::update(float dt) {
    // Mouse look
    float dx, dy;
    InputManager::getMouseDelta(dx, dy);
    yaw   += dx * lookSpeed;
    pitch -= dy * lookSpeed; // subtract so mouse-up = look up
    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Direction vectors from yaw/pitch
    float yr = yaw   * DEG2RAD;
    float pr = pitch * DEG2RAD;
    Vec3 forward = Vec3(cosf(pr) * cosf(yr), sinf(pr), cosf(pr) * sinf(yr)).normalized();
    Vec3 right   = forward.cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();

    // WASD + vertical movement
    float spd = moveSpeed * dt;
    if (InputManager::actionPressed("Forward"))  position = position + forward * spd;
    if (InputManager::actionPressed("Backward")) position = position - forward * spd;
    if (InputManager::actionPressed("Right"))    position = position + right   * spd;
    if (InputManager::actionPressed("Left"))     position = position - right   * spd;
    if (InputManager::actionPressed("Up"))       position.y += spd;
    if (InputManager::actionPressed("Down"))     position.y -= spd;

    rebuildView();
}

void Camera::rebuildView() {
    float yr = yaw   * DEG2RAD;
    float pr = pitch * DEG2RAD;
    Vec3 forward = Vec3(cosf(pr) * cosf(yr), sinf(pr), cosf(pr) * sinf(yr)).normalized();
    Vec3 right   = forward.cross(Vec3(0.0f, 1.0f, 0.0f)).normalized();
    Vec3 up      = right.cross(forward);
    _view = lookAt(position, position + forward, up);
}

void Camera::rebuildProjection() {
    _proj = perspective(fovDeg * DEG2RAD, _aspect, _zNear, _zFar);
}
