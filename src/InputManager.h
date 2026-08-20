#pragma once
#include <unordered_map>
#include <string>
#include <glad/glad.h> // must precede glfw3.h so GLFW doesn't pull in the system GL headers first
#include <GLFW/glfw3.h>

class InputManager {
public:
    static void initialize(GLFWwindow* window);
    static void update();
    static void mapActionToKey(const std::string& action, int key);
    static bool actionPressed(const std::string& action);
    static bool actionJustPressed(const std::string& action);
    static bool keyPressed(int key);
    static bool keyJustPressed(int key);
    static void getMouseDelta(float& dx, float& dy);
    static void setCursorCaptured(bool captured);
    static bool isCursorCaptured();
    static GLFWwindow* getWindow();
    static void setFramebufferSize(int width, int height);
    static void getFramebufferSize(int& width, int& height);

private:
    static std::unordered_map<int, bool>         _keyStates;
    static std::unordered_map<int, bool>         _previousKeyStates;
    static std::unordered_map<std::string, int>  _actionKeyMap;
    static GLFWwindow*                           _window;
    static double                                _mouseX;
    static double                                _mouseY;
    static float                                 _mouseDX;
    static float                                 _mouseDY;
    static bool                                  _cursorCaptured;
    static int                                   _fbWidth;
    static int                                   _fbHeight;
};
