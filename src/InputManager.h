#pragma once
#include <unordered_map>
#include <string>
#include <GLFW/glfw3.h>

class InputManager {
public:
    static void initialize(GLFWwindow* window);
    static void update();
    static void mapActionToKey(const std::string& action, int key);
    static bool actionPressed(const std::string& action);

private:
    static std::unordered_map<int, bool>         _keyStates;
    static std::unordered_map<int, bool>         _previousKeyStates;
    static std::unordered_map<std::string, int>  _actionKeyMap;
    static GLFWwindow*                           _window;
};
