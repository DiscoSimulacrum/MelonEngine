#include "Shader.h"
#include <iostream>

static const char* vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    out vec3 vWorldPos;
    out vec3 vNormal;
    out vec2 vTexCoord;

    void main() {
        vec4 worldPos   = uModel * vec4(aPos, 1.0);
        gl_Position     = uProjection * uView * worldPos;
        vWorldPos       = worldPos.xyz;
        vNormal         = mat3(transpose(inverse(uModel))) * aNormal;
        vTexCoord       = aTexCoord;
    }
)";

static const char* fragmentShaderSrc = R"(
    #version 330 core
    in  vec3 vWorldPos;
    in  vec3 vNormal;
    in  vec2 vTexCoord;
    out vec4 FragColor;

    uniform sampler2D uAlbedo;
    uniform vec3      uLightDir;   // world-space direction toward the light (unnormalized ok)
    uniform vec3      uLightColor;
    uniform vec3      uCameraPos;

    void main() {
        vec4 albedoSample = texture(uAlbedo, vTexCoord);
        vec3 albedo = albedoSample.rgb;

        vec3 N = normalize(vNormal);
        vec3 L = normalize(uLightDir);
        vec3 V = normalize(uCameraPos - vWorldPos);
        vec3 H = normalize(L + V);

        // Ambient
        vec3 ambient = 0.08 * albedo;

        // Diffuse
        float diff    = max(dot(N, L), 0.0);
        vec3  diffuse = diff * uLightColor * albedo;

        // Specular (Blinn-Phong)
        float spec    = pow(max(dot(N, H), 0.0), 64.0);
        vec3  specular = spec * uLightColor * 0.3;

        // Rim light: brightens edges facing away from the camera
        float rim = pow(1.0 - max(dot(N, V), 0.0), 3.0);
        vec3  rimLight = rim * uLightColor;

        FragColor = vec4(ambient + diffuse + specular + rimLight, albedoSample.a);
    }
)";

static GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Shader compile error:\n" << log << std::endl;
    }
    return shader;
}

static GLuint createProgram(const char* vertSrc, const char* fragSrc) {
    GLuint vert = compileShader(GL_VERTEX_SHADER,   vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Shader link error:\n" << log << std::endl;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

GLuint createLitShaderProgram() {
    return createProgram(vertexShaderSrc, fragmentShaderSrc);
}
