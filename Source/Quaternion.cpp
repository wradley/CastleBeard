#include <cmath>
#include "../Include/Math/Quaternion.h"
#include "../Include/Math/Vector.h"


Math::Quat::Quat()
{}


Math::Quat::Quat(const Vec3 & euler)
{
    float cx = cos(euler.x / 2.0f);
    float sx = sin(euler.x / 2.0f);
    float cy = cos(euler.y / 2.0f);
    float sy = sin(euler.y / 2.0f);
    float cz = cos(euler.z / 2.0f);
    float sz = sin(euler.z / 2.0f);
    x = ((sx * cy * cz) - (cx * sy * sz));
    y = ((cx * sy * cz) + (sx * cy * sz));
    z = ((cx * cy * sz) - (sx * sy * cz));
    w = ((cx * cy * cz) + (sx * sy * sz));
    Normalize(); // rounding errors
}


Math::Quat::Quat(const Vec4 & v) : x(v.x), y(v.y), z(v.z), w(v.w)
{}


Math::Quat::Quat(const Quat & q) : x(q.x), y(q.y), z(q.z), w(q.w)
{}


Math::Quat & Math::Quat::operator=(const Quat & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
}


Math::Quat::Quat(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
{}


Math::Quat::~Quat()
{}


Math::Quat Math::Quat::FromEuler(const Vec3 & v)
{
    float cx = cos(v.x / 2.0f);
    float sx = sin(v.x / 2.0f);
    float cy = cos(v.y / 2.0f);
    float sy = sin(v.y / 2.0f);
    float cz = cos(v.z / 2.0f);
    float sz = sin(v.z / 2.0f);

    return Quat(
        ((sx * cy * cz) - (cx * sy * sz)),
        ((cx * sy * cz) + (sx * cy * sz)),
        ((cx * cy * sz) - (sx * sy * cz)),
        ((cx * cy * cz) + (sx * sy * sz))
    );
}


Math::Quat Math::Quat::operator*(const Quat & q) const
{
    float a = w;
    float b = x;
    float c = y;
    float d = z;

    float e = q.w;
    float f = q.x;
    float g = q.y;
    float h = q.z;

    return Quat(
        (b*e) + (a*f) + (c*h) - (d*g),
        (a*g) - (b*h) + (c*e) + (d*f),
        (a*h) + (b*g) - (c*f) + (d*e),
        (a*e) - (b*f) - (c*g) - (d*h)
    );
}


void Math::Quat::Normalize()
{
    float d = sqrt((w*w) + (x*x) + (y*y) + (z*z));
    x /= d;
    y /= d;
    z /= d;
    w /= d;
}


Math::Quat Math::Normalize(const Quat & q)
{
    float d = sqrt((q.w*q.w) + (q.x*q.x) + (q.y*q.y) + (q.z*q.z));
    return Quat(q.x/d, q.y/d, q.z/d, q.w/d);
}
