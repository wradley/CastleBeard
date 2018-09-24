#include "../Include/Math/Vector.h"


Math::Vec2::Vec2() : x(0.0f), y(0.0f)
{}


Math::Vec2::Vec2(float X) : x(X), y(X)
{}


Math::Vec2::Vec2(float(&X)[2]) : x(X[0]), y(X[1])
{}


Math::Vec2::Vec2(float X, float Y) : x(X), y(Y)
{}


Math::Vec2::Vec2(const Vec2 &v) : x(v.x), y(v.y)
{}


Math::Vec2::Vec2(const Vec3 &v) : x(v.x), y(v.y)
{}


Math::Vec2::Vec2(const Vec4 &v) : x(v.x), y(v.y)
{}


Math::Vec2& Math::Vec2::operator= (const Vec2 &v) 
{
    x = v.x;
    y = v.y;
    return *this;
}


Math::Vec2::~Vec2()
{}


Math::Vec2 Math::Vec2::operator+ (const Vec2 &v) const 
{
    return Vec2(x + v.x, y + v.y);
}


Math::Vec2 Math::Vec2::operator- (const Vec2 &v) const 
{
    return Vec2(x - v.x, y - v.y);
}


Math::Vec2 Math::Vec2::operator* (float s) const 
{
    return Vec2(x * s, y * s);
}


Math::Vec2 Math::Vec2::operator/ (float s) const
{
    return Vec2(x / s, y / s);
}


Math::Vec2& Math::Vec2::operator+= (const Vec2 &v)
{
    x += v.x;
    y += v.y;
    return *this;
}


Math::Vec2& Math::Vec2::operator-= (const Vec2 &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}


Math::Vec2& Math::Vec2::operator*= (float s)
{
    x *= s;
    y *= s;
    return *this;
}


Math::Vec2& Math::Vec2::operator/= (float s)
{
    x /= s;
    y /= s;
    return *this;
}


// ============================================================================


Math::Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f)
{}


Math::Vec3::Vec3(float X) : x(X), y(X), z(X)
{}


Math::Vec3::Vec3(float(&X)[3]) : x(X[0]), y(X[1]), z(X[2])
{}


Math::Vec3::Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z)
{}


Math::Vec3::Vec3(const Vec2 & v) : x(v.x), y(v.y), z(0.0f)
{}


Math::Vec3::Vec3(const Vec2 & v, float Z) : x(v.x), y(v.y), z(Z)
{}


Math::Vec3::Vec3(const Vec3 & v) : x(v.x), y(v.y), z(v.z)
{}


Math::Vec3::Vec3(const Vec4 & v) : x(v.x), y(v.y), z(v.z)
{}


Math::Vec3& Math::Vec3::operator= (const Vec3 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}


Math::Vec3::~Vec3()
{}


Math::Vec3 Math::Vec3::operator+ (const Vec3 &v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}


Math::Vec3 Math::Vec3::operator- (const Vec3 &v) const 
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}


Math::Vec3 Math::Vec3::operator* (float s) const 
{
    return Vec3(x * s, y * s, z * s);
}


Math::Vec3 Math::Vec3::operator/ (float s) const
{
    return Vec3(x / s, y / s, z / s);
}


Math::Vec3& Math::Vec3::operator+= (const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}


Math::Vec3& Math::Vec3::operator-= (const Vec3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}


Math::Vec3& Math::Vec3::operator*= (float s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}


Math::Vec3& Math::Vec3::operator/= (float s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}


// ============================================================================


Math::Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{}


Math::Vec4::Vec4(float X) : x(X), y(X), z(X), w(X)
{}


Math::Vec4::Vec4(float(&X)[4]) : x(X[0]), y(X[1]), z(X[2]), w(X[3])
{}


Math::Vec4::Vec4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
{}


Math::Vec4::Vec4(const Vec2 &v) : x(v.x), y(v.y), z(0.0f), w(0.0f)
{}


Math::Vec4::Vec4(const Vec2 &v, float Z, float W) : x(v.x), y(v.y), z(Z), w(W)
{}


Math::Vec4::Vec4(const Vec3 &v) : x(v.x), y(v.y), z(v.z), w(0.0f)
{}


Math::Vec4::Vec4(const Vec3 &v, float W) : x(v.x), y(v.y), z(v.z), w(W)
{}


Math::Vec4::Vec4(const Vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w)
{}


Math::Vec4& Math::Vec4::operator= (const Vec4 &v) 
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}


Math::Vec4::~Vec4() 
{}


Math::Vec4 Math::Vec4::operator+ (const Vec4 &v) const 
{
    return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}


Math::Vec4 Math::Vec4::operator- (const Vec4 &v) const 
{
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}


Math::Vec4 Math::Vec4::operator* (float s) const
{
    return Vec4(x * s, y * s, z * s, w * s);
}


Math::Vec4 Math::Vec4::operator/ (float s) const
{
    return Vec4(x / s, y / s, z / s, w / s);
}


Math::Vec4& Math::Vec4::operator+= (const Vec4 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}


Math::Vec4& Math::Vec4::operator-= (const Vec4 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}


Math::Vec4& Math::Vec4::operator*= (float s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}


Math::Vec4& Math::Vec4::operator/= (float s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}