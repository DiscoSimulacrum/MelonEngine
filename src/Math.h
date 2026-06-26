#pragma once
#include <cmath>

struct Vec3 {
    float x = 0, y = 0, z = 0;

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
    Vec3 operator*(float s)       const { return Vec3(x*s,   y*s,   z*s);   }

    float length()     const { return sqrtf(x*x + y*y + z*z); }
    Vec3  normalized() const { float l = length(); return Vec3(x/l, y/l, z/l); }
    float dot(const Vec3& o)   const { return x*o.x + y*o.y + z*o.z; }
    Vec3  cross(const Vec3& o) const {
        return Vec3(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x);
    }
};

// Column-major 4x4 matrix (matches OpenGL / glUniformMatrix4fv default)
struct Mat4 {
    float m[16] = {};

    static Mat4 identity() {
        Mat4 r;
        r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
        return r;
    }

    Mat4 operator*(const Mat4& b) const {
        Mat4 r;
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row)
                for (int k = 0; k < 4; ++k)
                    r.m[col*4+row] += m[k*4+row] * b.m[col*4+k];
        return r;
    }
};

inline Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 worldUp) {
    Vec3 f = (center - eye).normalized();
    Vec3 r = f.cross(worldUp).normalized();
    Vec3 u = r.cross(f);

    Mat4 m;
    m.m[ 0] =  r.x;  m.m[ 4] =  r.y;  m.m[ 8] =  r.z;  m.m[12] = -r.dot(eye);
    m.m[ 1] =  u.x;  m.m[ 5] =  u.y;  m.m[ 9] =  u.z;  m.m[13] = -u.dot(eye);
    m.m[ 2] = -f.x;  m.m[ 6] = -f.y;  m.m[10] = -f.z;  m.m[14] =  f.dot(eye);
    m.m[ 3] =  0;    m.m[ 7] =  0;    m.m[11] =  0;     m.m[15] =  1;
    return m;
}

// fovRad: vertical field of view in radians
inline Mat4 perspective(float fovRad, float aspect, float zNear, float zFar) {
    float f = 1.0f / tanf(fovRad * 0.5f);
    Mat4 m;
    m.m[ 0] = f / aspect;
    m.m[ 5] = f;
    m.m[10] = (zFar + zNear) / (zNear - zFar);
    m.m[11] = -1.0f;
    m.m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    return m;
}
