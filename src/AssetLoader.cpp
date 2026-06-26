#include "AssetLoader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/stb/stb_image.h"

// =====================================================================
// OBJ loader
// =====================================================================

struct OBJIndex {
    int pos = -1, tc = -1, nrm = -1;
    bool operator==(const OBJIndex& o) const {
        return pos == o.pos && tc == o.tc && nrm == o.nrm;
    }
};

struct OBJIndexHash {
    size_t operator()(const OBJIndex& i) const {
        size_t h = std::hash<int>()(i.pos);
        h ^= std::hash<int>()(i.tc)  + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>()(i.nrm) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

static OBJIndex parseOBJIndex(const std::string& token) {
    OBJIndex idx;
    std::istringstream ss(token);
    std::string part;
    int field = 0;
    while (std::getline(ss, part, '/')) {
        if (!part.empty()) {
            int val = std::stoi(part) - 1; // OBJ indices are 1-based
            if      (field == 0) idx.pos = val;
            else if (field == 1) idx.tc  = val;
            else if (field == 2) idx.nrm = val;
        }
        ++field;
    }
    return idx;
}

static unsigned int emitVertex(
    const OBJIndex& fi,
    const std::vector<std::array<float, 3>>& positions,
    const std::vector<std::array<float, 3>>& normals,
    const std::vector<std::array<float, 2>>& texcoords,
    std::vector<float>& verts,
    std::unordered_map<OBJIndex, unsigned int, OBJIndexHash>& cache)
{
    auto it = cache.find(fi);
    if (it != cache.end()) return it->second;

    unsigned int idx = static_cast<unsigned int>(verts.size() / 8);

    const auto& p = positions[fi.pos];
    verts.push_back(p[0]); verts.push_back(p[1]); verts.push_back(p[2]);

    if (fi.nrm >= 0 && fi.nrm < static_cast<int>(normals.size())) {
        const auto& n = normals[fi.nrm];
        verts.push_back(n[0]); verts.push_back(n[1]); verts.push_back(n[2]);
    } else {
        verts.push_back(0.0f); verts.push_back(0.0f); verts.push_back(1.0f);
    }

    if (fi.tc >= 0 && fi.tc < static_cast<int>(texcoords.size())) {
        const auto& tc = texcoords[fi.tc];
        verts.push_back(tc[0]); verts.push_back(tc[1]);
    } else {
        verts.push_back(0.0f); verts.push_back(0.0f);
    }

    cache[fi] = idx;
    return idx;
}

Mesh loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "loadOBJ: cannot open " << path << "\n";
        return {};
    }

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 3>> normals;
    std::vector<std::array<float, 2>> texcoords;
    std::vector<float>                verts;
    std::vector<unsigned int>         indices;
    std::unordered_map<OBJIndex, unsigned int, OBJIndexHash> cache;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            std::array<float, 3> v;
            ss >> v[0] >> v[1] >> v[2];
            positions.push_back(v);
        } else if (prefix == "vt") {
            std::array<float, 2> vt;
            ss >> vt[0] >> vt[1];
            texcoords.push_back(vt);
        } else if (prefix == "vn") {
            std::array<float, 3> vn;
            ss >> vn[0] >> vn[1] >> vn[2];
            normals.push_back(vn);
        } else if (prefix == "f") {
            // Collect all tokens on this face line, then fan-triangulate
            std::vector<OBJIndex> face;
            std::string token;
            while (ss >> token) face.push_back(parseOBJIndex(token));

            for (size_t i = 1; i + 1 < face.size(); ++i) {
                indices.push_back(emitVertex(face[0],   positions, normals, texcoords, verts, cache));
                indices.push_back(emitVertex(face[i],   positions, normals, texcoords, verts, cache));
                indices.push_back(emitVertex(face[i+1], positions, normals, texcoords, verts, cache));
            }
        }
    }

    if (positions.empty() || indices.empty()) {
        std::cerr << "loadOBJ: no geometry in " << path << "\n";
        return {};
    }

    Mesh mesh;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verts.size() * sizeof(float)),
        verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(), GL_STATIC_DRAW);

    constexpr GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    mesh.indexCount = static_cast<int>(indices.size());
    return mesh;
}

// =====================================================================
// Texture loader
// =====================================================================

Texture loadTexture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true); // OpenGL expects origin at bottom-left
    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (!data) {
        std::cerr << "loadTexture: cannot load " << path << "\n";
        return {};
    }

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    Texture tex;
    tex.width  = w;
    tex.height = h;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format),
                 w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

// =====================================================================
// Cleanup
// =====================================================================

void freeMesh(Mesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    mesh = {};
}

void freeTexture(Texture& tex) {
    glDeleteTextures(1, &tex.id);
    tex = {};
}
