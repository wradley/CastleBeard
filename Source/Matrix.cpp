#include "../Include/Math/Matrix.h"
#include "../Include/Math/Vector.h"


Math::Mat2::Mat2() : 
    values{
        0.0f, 0.0f, 
        0.0f, 0.0f  }
{}


Math::Mat2::Mat2(float i) :
    values{
           i, 0.0f,
        0.0f,    i  }
{}


Math::Mat2::Mat2(const Mat2 & m) :
    values{
        m.values[0], m.values[1],
        m.values[2], m.values[3] }
{}


Math::Mat2::Mat2(const Mat3 & m) :
    values{
        m.values[0], m.values[1],
        m.values[2], m.values[4] }
{}


Math::Mat2::Mat2(const Mat4 & m) :
    values{
        m.values[0], m.values[1],
        m.values[4], m.values[5] }
{}


Math::Mat2 Math::Mat2::operator*(float s) const
{
    Mat2 r;
    r.values[0] = values[0] * s;
    r.values[1] = values[1] * s;
    r.values[2] = values[2] * s;
    r.values[3] = values[3] * s;
    return r;
}


Math::Vec2 Math::Mat2::operator*(const Vec2 & v) const
{
    float x, y;
    x = (values[0] * v.x) + (values[2] * v.y);
    y = (values[1] * v.x) + (values[3] * v.y);
    return Vec2(x, y);
}


Math::Mat2 Math::Mat2::operator*(const Mat2 & v) const
{
    auto &othr = v.values;
    auto &me = values;

    Mat2 r;
    r.values[0] = (me[0] * othr[0]) + (me[2] * othr[1]);
    r.values[1] = (me[1] * othr[0]) + (me[3] * othr[1]);
    r.values[2] = (me[0] * othr[2]) + (me[2] * othr[3]);
    r.values[3] = (me[1] * othr[2]) + (me[3] * othr[3]);
    return r;
}


Math::Mat2 & Math::Mat2::operator*=(float s)
{
    values[0] *= s;
    values[1] *= s;
    values[2] *= s;
    values[3] *= s;
    return *this;
}
