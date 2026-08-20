#pragma once
#include <glad/glad.h>
#include "MelonMath.h"

// Must match MAX_POINT_LIGHTS in the fragment shader source (Shader.cpp).
constexpr int MAX_POINT_LIGHTS = 4;

struct PointLight {
    Vec3 position;
    Vec3 color;
};

// Compiles/links the engine's shared Blinn-Phong lit shader with point lights.
// Attributes: location 0 = position, 1 = normal, 2 = texcoord.
// Uniforms: uModel, uView, uProjection, uAlbedo (sampler2D), uCameraPos,
//           uLightCount, uLightPos[MAX_POINT_LIGHTS], uLightColor[MAX_POINT_LIGHTS].
GLuint createLitShaderProgram();

// Uploads up to MAX_POINT_LIGHTS lights (extras beyond the cap are ignored).
void setPointLights(GLuint program, const PointLight* lights, int count);
