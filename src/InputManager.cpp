#include "InputManager.h"
#include <iostream>

std::unordered_map<int, bool> InputManager::_keyStates;
std::unordered_map<int, bool> InputManager::_previousKeyStates;
std::unordered_map<std::string, int> InputManager::_actionKeyMap;
GLFWwindow* InputManager::_window;

void InputManager::initialize(GLFWwindow* window) {
    if (!window) {
        std::cerr << "[InputManager] Error: GLFW window is not initialized!" << std::endl;
        return;
    }
    

    // Assign the window pointer
    _window = window;
    
    // Clear hashtables
    _keyStates.clear();
    _previousKeyStates.clear();

    //Map actions to keys
    mapActionToKey("Forward", GLFW_KEY_W);
    mapActionToKey("Backward", GLFW_KEY_S);
    mapActionToKey("Left", GLFW_KEY_A);
    mapActionToKey("Right", GLFW_KEY_D);
}

// Update keyStates based on input
void InputManager::update() {
    if (_window == nullptr) {
        std::cerr << "[InputManager] Error: Invalid GLFW window pointer!" << std::endl;
        return;
    }   
    
    _previousKeyStates = _keyStates;

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        int state = glfwGetKey(_window, key);

        // Update the key state
        if (state == GLFW_PRESS) {
            _keyStates[key] = true;  // Key is pressed
        }
        else {
            _keyStates[key] = false;  // Key is not pressed
        }
    }
    
}

void InputManager::mapActionToKey(const std::string& action, int key) {
    _actionKeyMap[action] = key;
}

bool InputManager::actionPressed(const std::string& action) {
    int key = _actionKeyMap[action];
    return _keyStates[key];
}