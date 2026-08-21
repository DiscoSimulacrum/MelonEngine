#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <functional>

// Caches compiled/linked shader programs by name so scene transitions reuse
// an already-compiled program instead of recompiling it every time.
// Owned by SceneManager and torn down once at engine shutdown; scenes only
// borrow handles from it and must not glDeleteProgram them.
class ShaderCache {
public:
    using Factory = std::function<GLuint()>;

    GLuint getOrCreate(const std::string& name, const Factory& factory);
    void   shutdown();

private:
    std::unordered_map<std::string, GLuint> _programs;
};
