#pragma once
#include <glad/glad.h>
#include "MelonMath.h"

// Must match MAX_POINT_LIGHTS in the fragment shader source (Shader.cpp).
constexpr int MAX_POINT_LIGHTS = 4;

struct PointLight {
    Vec3 position;
    Vec3 color;
};

// Linear distance fog: fully clear at/before `start`, fully `color` at/after `end`.
struct Fog {
    Vec3  color;
    float start;
    float end;
};

// Selects which shader program a mesh renders with. Mutually exclusive by
// nature (a mesh is lit or emissive, never a blend of both), unlike a
// continuous parameter such as opacity.
enum class MaterialType {
    Lit,
    Emissive
};

// Compiles/links the engine's shared Blinn-Phong lit shader with point lights
// and optional linear distance fog.
// Attributes: location 0 = position, 1 = normal, 2 = texcoord.
// Uniforms: uModel, uView, uProjection, uAlbedo (sampler2D), uCameraPos,
//           uLightCount, uLightPos[MAX_POINT_LIGHTS], uLightColor[MAX_POINT_LIGHTS],
//           uFogEnabled, uFogColor, uFogStart, uFogEnd.
GLuint createLitShaderProgram();

// Compiles/links an unlit emissive shader: outputs uAlbedo * uEmissiveIntensity
// with no lighting calculation. uAlbedo works the same as in the lit shader,
// so it accepts both loadTexture() images and loadSolidColorTexture() swatches.
// Attributes: location 0 = position, 2 = texcoord (normal attribute is unused).
// Uniforms: uModel, uView, uProjection, uAlbedo (sampler2D), uEmissiveIntensity.
GLuint createEmissiveShaderProgram();

// Uploads up to MAX_POINT_LIGHTS lights (extras beyond the cap are ignored).
void setPointLights(GLuint program, const PointLight* lights, int count);

// Configures linear distance fog on the lit shader. uFogEnabled is GL program
// state shared across every scene that borrows this program via ShaderCache,
// so any scene using the lit shader must call this once in init() even just
// to pass enabled=false, or it will silently inherit whatever fog settings
// the previously active scene left behind.
void setFog(GLuint program, const Fog& fog, bool enabled);
