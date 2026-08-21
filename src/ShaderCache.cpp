#include "ShaderCache.h"

GLuint ShaderCache::getOrCreate(const std::string& name, const Factory& factory) {
    auto it = _programs.find(name);
    if (it != _programs.end()) return it->second;

    GLuint program = factory();
    _programs[name] = program;
    return program;
}

void ShaderCache::shutdown() {
    for (auto& entry : _programs) {
        glDeleteProgram(entry.second);
    }
    _programs.clear();
}
