#include "Shader.h"
#include <iostream>
#include <string>

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
    #define MAX_POINT_LIGHTS 4

    in  vec3 vWorldPos;
    in  vec3 vNormal;
    in  vec2 vTexCoord;
    out vec4 FragColor;

    uniform sampler2D uAlbedo;
    uniform vec3      uCameraPos;

    uniform int  uLightCount;
    uniform vec3 uLightPos[MAX_POINT_LIGHTS];
    uniform vec3 uLightColor[MAX_POINT_LIGHTS];

    void main() {
        vec4 albedoSample = texture(uAlbedo, vTexCoord);
        vec3 albedo = albedoSample.rgb;

        vec3 N = normalize(vNormal);
        vec3 V = normalize(uCameraPos - vWorldPos);

        // Ambient
        vec3 result = 0.08 * albedo;

        for (int i = 0; i < uLightCount; ++i) {
            vec3  toLight = uLightPos[i] - vWorldPos;
            float dist    = length(toLight);
            vec3  L       = toLight / dist;
            vec3  H       = normalize(L + V);
            float atten   = 1.0 / (1.0 + 0.045 * dist + 0.0075 * dist * dist);

            // Diffuse
            float diff    = max(dot(N, L), 0.0);
            vec3  diffuse = diff * uLightColor[i] * albedo;

            // Specular (Blinn-Phong)
            float spec     = pow(max(dot(N, H), 0.0), 64.0);
            vec3  specular = spec * uLightColor[i] * 0.3;

            result += (diffuse + specular) * atten;
        }

        FragColor = vec4(result, albedoSample.a);
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

void setPointLights(GLuint program, const PointLight* lights, int count) {
    if (count > MAX_POINT_LIGHTS) count = MAX_POINT_LIGHTS;

    glUniform1i(glGetUniformLocation(program, "uLightCount"), count);
    for (int i = 0; i < count; ++i) {
        std::string posName   = "uLightPos["   + std::to_string(i) + "]";
        std::string colorName = "uLightColor[" + std::to_string(i) + "]";
        glUniform3f(glGetUniformLocation(program, posName.c_str()),
            lights[i].position.x, lights[i].position.y, lights[i].position.z);
        glUniform3f(glGetUniformLocation(program, colorName.c_str()),
            lights[i].color.x, lights[i].color.y, lights[i].color.z);
    }
}
