#pragma once
#include <string>
#include "Mesh.h"
#include "Texture.h"

// Loads an OBJ file and uploads it to the GPU. If groupName is non-empty,
// only faces under the matching "o <groupName>" section are included -
// for files that bundle multiple named parts into one export sharing a
// vertex pool. Leave empty to load every face in the file (the common case).
// Vertex layout: location 0 = position (vec3), 1 = normal (vec3), 2 = texcoord (vec2).
// Returns a zeroed Mesh on failure (including a groupName with no match).
Mesh loadOBJ(const std::string& path, const std::string& groupName = "");

// Loads a texture (PNG/JPG/BMP/TGA) via stb_image and uploads it to the GPU.
// Returns a zeroed Texture on failure.
Texture loadTexture(const std::string& path);

// Uploads a 1x1 solid-color texture, for meshes with no source texture.
Texture loadSolidColorTexture(unsigned char r, unsigned char g, unsigned char b);

void freeMesh(Mesh& mesh);
void freeTexture(Texture& tex);
