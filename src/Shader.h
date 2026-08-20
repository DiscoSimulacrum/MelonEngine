#pragma once
#include <glad/glad.h>

// Compiles/links the engine's shared Blinn-Phong lit shader.
// Attributes: location 0 = position, 1 = normal, 2 = texcoord.
// Uniforms: uModel, uView, uProjection, uAlbedo (sampler2D), uLightDir, uLightColor, uCameraPos.
GLuint createLitShaderProgram();
