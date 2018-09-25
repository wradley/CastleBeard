#include <cmath>
#include "../Include/Math/Matrix.h"
#include "../Include/Math/Vector.h"
#include "../Include/Math/Quaternion.h"

Math::Mat4::Mat4() : values{
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
} {}


Math::Mat4::Mat4(float i) : values{
    i, 0.0f, 0.0f, 0.0f,
    0.0f, i, 0.0f, 0.0f,
    0.0f, 0.0f, i, 0.0f,
    0.0f, 0.0f, 0.0f, i
} {}


Math::Mat4::Mat4(const float(&v)[16]) : values{
    v[ 0], v[ 1], v[ 2], v[ 3],
    v[ 4], v[ 5], v[ 6], v[ 7],
    v[ 8], v[ 9], v[10], v[11],
    v[12], v[13], v[14], v[15]
} {}


Math::Mat4::Mat4(const Mat4 & m) : values{
    m.values[ 0], m.values[ 1], m.values[ 2], m.values[ 3],
    m.values[ 4], m.values[ 5], m.values[ 6], m.values[ 7],
    m.values[ 8], m.values[ 9], m.values[10], m.values[11],
    m.values[12], m.values[13], m.values[14], m.values[15]
} {}


Math::Mat4 & Math::Mat4::operator=(const Mat4 & m)
{
    values[ 0] = m.values[ 0];
    values[ 1] = m.values[ 1];
    values[ 2] = m.values[ 2];
    values[ 3] = m.values[ 3];
    values[ 4] = m.values[ 4];
    values[ 5] = m.values[ 5];
    values[ 6] = m.values[ 6];
    values[ 7] = m.values[ 7];
    values[ 8] = m.values[ 8];
    values[ 9] = m.values[ 9];
    values[10] = m.values[10];
    values[11] = m.values[11];
    values[12] = m.values[12];
    values[13] = m.values[13];
    values[14] = m.values[14];
    values[15] = m.values[15];
    return *this;
}


Math::Mat4::~Mat4()
{}


Math::Mat4 Math::Mat4::FromQuat(const Quat & q)
{
    float m_00 = 1 - 2 * (q.y*q.y) - 2 * (q.z*q.z);
    float m_10 = 2 * q.x * q.y - 2 * q.z * q.w;
    float m_20 = 2 * q.x * q.z + 2 * q.y * q.w;

    float m_01 = 2 * q.x * q.y + 2 * q.z * q.w;
    float m_11 = 1 - 2 * (q.x*q.x) - 2 * (q.z*q.z);
    float m_21 = 2 * q.y * q.z - 2 * q.x * q.w;

    float m_02 = 2 * q.x * q.z - 2 * q.y * q.w;
    float m_12 = 2 * q.y * q.z + 2 * q.x * q.w;
    float m_22 = 1 - 2 * (q.x*q.x) - 2 * (q.y*q.y);

    return Mat4({
        m_00, m_01, m_02, 0.0f,
        m_10, m_11, m_12, 0.0f,
        m_20, m_21, m_22, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
}


Math::Mat4 Math::Mat4::operator*(float s) const
{
    Mat4 ret;
    ret.values[ 0] = values[ 0] * s;
    ret.values[ 1] = values[ 1] * s;
    ret.values[ 2] = values[ 2] * s;
    ret.values[ 3] = values[ 3] * s;
    ret.values[ 4] = values[ 4] * s;
    ret.values[ 5] = values[ 5] * s;
    ret.values[ 6] = values[ 6] * s;
    ret.values[ 7] = values[ 7] * s;
    ret.values[ 8] = values[ 8] * s;
    ret.values[ 9] = values[ 9] * s;
    ret.values[10] = values[10] * s;
    ret.values[11] = values[11] * s;
    ret.values[12] = values[12] * s;
    ret.values[13] = values[13] * s;
    ret.values[14] = values[14] * s;
    ret.values[15] = values[15] * s;
    return ret;
}


Math::Vec4 Math::Mat4::operator*(const Vec4 & v) const
{
    Vec4 ret;
    auto &m = values; // me

    ret.x = (m[0] * v.x) + (m[4] * v.y) + (m[ 8] * v.z) + (m[12] * v.w);
    ret.y = (m[1] * v.x) + (m[5] * v.y) + (m[ 9] * v.z) + (m[13] * v.w);
    ret.z = (m[2] * v.x) + (m[6] * v.y) + (m[10] * v.z) + (m[14] * v.w);
    ret.w = (m[3] * v.x) + (m[7] * v.y) + (m[11] * v.z) + (m[15] * v.w);
    return ret;
}


Math::Mat4 Math::Mat4::operator*(const Mat4 & other) const
{
    Mat4 rtrn;
    auto &r = rtrn.values;  // return
    auto &m = values;       // me
    auto &o = other.values; // other

    r[ 0] = (m[0] * o[ 0]) + (m[4] * o[ 1]) + (m[ 8] * o[ 2]) + (m[12] * o[ 3]);
    r[ 4] = (m[0] * o[ 4]) + (m[4] * o[ 5]) + (m[ 8] * o[ 6]) + (m[12] * o[ 7]);
    r[ 8] = (m[0] * o[ 8]) + (m[4] * o[ 9]) + (m[ 8] * o[10]) + (m[12] * o[11]);
    r[12] = (m[0] * o[12]) + (m[4] * o[13]) + (m[ 8] * o[14]) + (m[12] * o[15]);

    r[ 1] = (m[1] * o[ 0]) + (m[5] * o[ 1]) + (m[ 9] * o[ 2]) + (m[13] * o[ 3]);
    r[ 5] = (m[1] * o[ 4]) + (m[5] * o[ 5]) + (m[ 9] * o[ 6]) + (m[13] * o[ 7]);
    r[ 9] = (m[1] * o[ 8]) + (m[5] * o[ 9]) + (m[ 9] * o[10]) + (m[13] * o[11]);
    r[13] = (m[1] * o[12]) + (m[5] * o[13]) + (m[ 9] * o[14]) + (m[13] * o[15]);

    r[ 2] = (m[2] * o[ 0]) + (m[6] * o[ 1]) + (m[10] * o[ 2]) + (m[14] * o[ 3]);
    r[ 6] = (m[2] * o[ 4]) + (m[6] * o[ 5]) + (m[10] * o[ 6]) + (m[14] * o[ 7]);
    r[10] = (m[2] * o[ 8]) + (m[6] * o[ 9]) + (m[10] * o[10]) + (m[14] * o[11]);
    r[14] = (m[2] * o[12]) + (m[6] * o[13]) + (m[10] * o[14]) + (m[14] * o[15]);

    r[ 3] = (m[3] * o[ 0]) + (m[7] * o[ 1]) + (m[11] * o[ 2]) + (m[15] * o[ 3]);
    r[ 7] = (m[3] * o[ 4]) + (m[7] * o[ 5]) + (m[11] * o[ 6]) + (m[15] * o[ 7]);
    r[11] = (m[3] * o[ 8]) + (m[7] * o[ 9]) + (m[11] * o[10]) + (m[15] * o[11]);
    r[15] = (m[3] * o[12]) + (m[7] * o[13]) + (m[11] * o[14]) + (m[15] * o[15]);

    return rtrn;
}


Math::Mat4 Math::Perspective(float fov, float aspect, float nearplane, float farplane)
{
    float tanHalfFov = tan(fov / 2.0f);
    float planeRange = nearplane - farplane;

    float m_00 = 1.0f / (tanHalfFov * aspect);
    float m_11 = 1.0f / tanHalfFov;
    float m_22 = (-nearplane - farplane) / planeRange;
    float m_23 = 2.0f * farplane * nearplane / planeRange;

/*    return Mat4({
        m_00, 0.0f, 0.0f, 0.0f,
        0.0f, m_11, 0.0f, 0.0f,
        0.0f, 0.0f, m_22, m_23,
        0.0f, 0.0f, 1.0f, 0.0f
    });
*/
    
    return Mat4({
        m_00, 0.0f, 0.0f, 0.0f,
        0.0f, m_11, 0.0f, 0.0f,
        0.0f, 0.0f, m_22, 1.0f,
        0.0f, 0.0f, m_23, 0.0f
    });
}


Math::Mat4 Math::LookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
    return Mat4();
}
