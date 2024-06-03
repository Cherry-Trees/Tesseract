#ifndef _VEC_4_
#define _VEC_4_
#include <math.h>

struct Vec4 { float x, y, z, w; };
struct Mat4 { float i00, i01, i02, i03,
                    i10, i11, i12, i13,
                    i20, i21, i22, i23,
                    i30, i31, i32, i33; };

struct Vec4 multiply(const Vec4 v, const Mat4 m) {
    return {
        v.x * m.i00 + v.y * m.i10 + v.z * m.i20 + v.w * m.i30,
        v.x * m.i01 + v.y * m.i11 + v.z * m.i21 + v.w * m.i31,
        v.x * m.i02 + v.y * m.i12 + v.z * m.i22 + v.w * m.i32,
        v.x * m.i03 + v.y * m.i13 + v.z * m.i23 + v.w * m.i33,        
    };
}

struct Vec4 normalizeW(const Vec4 v) {
    return {
        v.x / v.w,
        v.y / v.w,
        v.z / v.w,
        1
    };
}

struct Vec4 mapW(const Vec4 v) {
    const float scalar = powf(1.35f, v.w);
    return {
        v.x * scalar,
        v.y * scalar,
        v.z * scalar,
        v.w
    };
}

struct Mat4 mat4RotationZW(const float sin_th, const float cos_th) {
    return {
        cos_th,     -sin_th,    0,          0,
        sin_th,     cos_th,     0,          0,
        0,          0,          1,          0,
        0,          0,          0,          1,
    };
}

struct Mat4 mat4RotationYW(const float sin_th, const float cos_th) {
    return {
        cos_th,     0,          -sin_th,    0,
        0,          1,          0,          0,
        sin_th,     0,          cos_th,     0,
        0,          0,          0,          1,
    };
}

struct Mat4 mat4RotationYZ(const float sin_th, const float cos_th) {
    return {
        cos_th,     0,          0,          -sin_th,
        0,          1,          0,          0,
        0,          0,          1,          0,
        sin_th,     0,          0,          cos_th,
    };
}

struct Mat4 mat4RotationXW(const float sin_th, const float cos_th) {
    return {
        1,          0,          0,          0,
        0,          cos_th,     -sin_th,    0, 
        0,          sin_th,     cos_th,     0,
        0,          0,          0,          1,
    };
}

struct Mat4 mat4RotationXZ(const float sin_th, const float cos_th) {
    return {
        1,          0,          0,          0,
        0,          cos_th,     0,          -sin_th,
        0,          0,          1,          0,
        0,          sin_th,     0,          cos_th,
    };
}

struct Mat4 mat4RotationXY(const float sin_th, const float cos_th) {
    return {
        1,          0,          0,          0,
        0,          1,          0,          0,
        0,          0,          cos_th,     -sin_th,
        0,          0,          sin_th,     cos_th,
    };
}

struct Mat4 mat4Projection(const float W, const float H, const float FOV = 3.14159f / 2.f, const float near = 0.01, const float far = 100)
{
    const float aspect_ratio = W / H;
    const float focal_length = 1.f / tanf(FOV / 2.f);

    const float i00 = focal_length * aspect_ratio;
    const float i11 = focal_length;
    const float i22 = far / (far - near);
    const float i32 = (-far * near) / (far - near);
    
    return {
        i00,    0,      0,      0,
        0,      i11,    0,      0,
        0,      0,      i22,    1,
        0,      0,      i32,    0,
    };
}

#endif
