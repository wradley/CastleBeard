#pragma once

namespace Math
{
    class Vec2;
    class Vec3;
    class Vec4;
    class Mat3;
    class Mat4;

    class Mat2
    {
    public:

        Mat2();
        Mat2(float i);
        Mat2(const Mat2 &m);
        Mat2(const Mat3 &m);
        Mat2(const Mat4 &m);

        Mat2 operator* (float s) const;
        Vec2 operator* (const Vec2 &v) const;
        Mat2 operator* (const Mat2 &v) const;

        Mat2& operator*= (float s);

        float values[4];
    };

    class Mat3
    {
    public:

        /*Mat3();
        Mat3(float i);

        Mat3 operator* (float s) const;
        Mat3 operator* (const Vec3 &v) const;
        Mat3 operator* (const Mat3 &v) const;*/
        
        float values[9];

    };

    class Mat4
    {
    public:

        float values[16];
    };
}