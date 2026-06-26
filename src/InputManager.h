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
    static void getMouseDelta(float& dx, float& dy);
    static void setCursorCaptured(bool captured);

private:
    static std::unordered_map<int, bool>         _keyStates;
    static std::unordered_map<int, bool>         _previousKeyStates;
    static std::unordered_map<std::string, int>  _actionKeyMap;
    static GLFWwindow*                           _window;
    static double                                _mouseX;
    static double                                _mouseY;
    static float                                 _mouseDX;
    static float                                 _mouseDY;
};
