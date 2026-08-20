#include "InputManager.h"
#include <iostream>

std::unordered_map<int, bool> InputManager::_keyStates;
std::unordered_map<int, bool> InputManager::_previousKeyStates;
std::unordered_map<std::string, int> InputManager::_actionKeyMap;
GLFWwindow* InputManager::_window  = nullptr;
double      InputManager::_mouseX  = 0.0;
double      InputManager::_mouseY  = 0.0;
float       InputManager::_mouseDX = 0.0f;
float       InputManager::_mouseDY = 0.0f;
bool        InputManager::_cursorCaptured = false;
int         InputManager::_fbWidth  = 0;
int         InputManager::_fbHeight = 0;

void InputManager::initialize(GLFWwindow* window) {
    if (!window) {
        std::cerr << "[InputManager] Error: GLFW window is not initialized!" << std::endl;
        return;
    }

    _window = window;
    _keyStates.clear();
    _previousKeyStates.clear();

    // Seed mouse position so the first frame has zero delta
    glfwGetCursorPos(_window, &_mouseX, &_mouseY);

    // Framebuffer size in pixels can differ from the window size passed to
    // glfwCreateWindow (DPI scaling), so read it back rather than assuming.
    glfwGetFramebufferSize(_window, &_fbWidth, &_fbHeight);

    mapActionToKey("Forward",  GLFW_KEY_W);
    mapActionToKey("Backward", GLFW_KEY_S);
    mapActionToKey("Left",     GLFW_KEY_A);
    mapActionToKey("Right",    GLFW_KEY_D);
    mapActionToKey("Up",       GLFW_KEY_SPACE);
    mapActionToKey("Down",     GLFW_KEY_LEFT_CONTROL);
    mapActionToKey("ToggleMouseCapture", GLFW_KEY_ESCAPE);
}

void InputManager::update() {
    if (_window == nullptr) {
        std::cerr << "[InputManager] Error: Invalid GLFW window pointer!" << std::endl;
        return;
    }

    _previousKeyStates = _keyStates;

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        _keyStates[key] = (glfwGetKey(_window, key) == GLFW_PRESS);
    }

    // Mouse delta
    double mx, my;
    glfwGetCursorPos(_window, &mx, &my);
    _mouseDX = static_cast<float>(mx - _mouseX);
    _mouseDY = static_cast<float>(my - _mouseY);
    _mouseX  = mx;
    _mouseY  = my;

    if (actionJustPressed("ToggleMouseCapture")) {
        setCursorCaptured(!_cursorCaptured);
    }
}

void InputManager::mapActionToKey(const std::string& action, int key) {
    _actionKeyMap[action] = key;
}

bool InputManager::actionPressed(const std::string& action) {
    return _keyStates[_actionKeyMap[action]];
}

bool InputManager::actionJustPressed(const std::string& action) {
    int key = _actionKeyMap[action];
    return _keyStates[key] && !_previousKeyStates[key];
}

bool InputManager::keyPressed(int key) {
    return _keyStates[key];
}

bool InputManager::keyJustPressed(int key) {
    return _keyStates[key] && !_previousKeyStates[key];
}

void InputManager::getMouseDelta(float& dx, float& dy) {
    dx = _mouseDX;
    dy = _mouseDY;
}

void InputManager::setCursorCaptured(bool captured) {
    _cursorCaptured = captured;
    glfwSetInputMode(_window, GLFW_CURSOR,
        captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    // Re-seed cursor position so re-capturing doesn't produce a one-frame look jump
    if (captured) {
        glfwGetCursorPos(_window, &_mouseX, &_mouseY);
    }
}

bool InputManager::isCursorCaptured() {
    return _cursorCaptured;
}

GLFWwindow* InputManager::getWindow() {
    return _window;
}

void InputManager::setFramebufferSize(int width, int height) {
    _fbWidth  = width;
    _fbHeight = height;
}

void InputManager::getFramebufferSize(int& width, int& height) {
    width  = _fbWidth;
    height = _fbHeight;
}
