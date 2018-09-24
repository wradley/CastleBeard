#pragma once

namespace Math
{
    class Vec2;
    class Vec3;
    class Vec4;
    class Mat2;
    class Mat3;
    class Mat4;

    Mat3 RotateEuler(const Mat3 &m, float x, float y, float z);
    Mat4 RotateEuler(const Mat4 &m, float x, float y, float z);
    Mat4 Transpose(const Mat4 &m, const Vec3 &v);

    class Mat2
    {
    public:

        Mat2();
        Mat2(float i);
        Mat2(const float (&i)[4]);
        Mat2(const Mat2 &m);
        Mat2(const Mat3 &m);
        Mat2(const Mat4 &m);
        Mat2& operator= (const Mat2 &m);

        Mat2 operator* (float s) const;
        Vec2 operator* (const Vec2 &v) const;
        Mat2 operator* (const Mat2 &v) const;

        float values[4];
    };

    class Mat3
    {
    public:

        Mat3();
        Mat3(float i);
        Mat3(const float(&i)[9]); 
        Mat3(const Mat2 &m);
        Mat3(const Mat3 &m);
        Mat3(const Mat4 &m);
        Mat3& operator= (const Mat3 &m);

        Mat3 operator* (float s) const;
        Vec3 operator* (const Vec3 &v) const;
        Mat3 operator* (const Mat3 &m) const;
        
        float values[9];

    };

    class Mat4
    {
    public:

        Mat4();
        Mat4(float i);
        Mat4(const float(&i)[16]);
        Mat4(const Mat2 &m);
        Mat4(const Mat3 &m);
        Mat4(const Mat4 &m);
        Mat4& operator= (const Mat4 &m);

        Mat4 operator* (float s) const;
        Vec4 operator* (const Vec4 &v) const;
        Mat4 operator* (const Mat4 &m) const;

        float values[16];
    };
}