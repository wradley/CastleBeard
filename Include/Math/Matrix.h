#pragma once

namespace Math
{
    class Quat;
    class Vec4;

    class Mat4
    {
    public:

        Mat4();
        Mat4(float i);
        Mat4(const float(&v)[16]);
        Mat4(const Mat4 &m);
        Mat4& operator= (const Mat4 &m);
        ~Mat4();

        static Mat4 FromQuat(const Quat &q);

        Mat4 operator* (float s) const;
        Vec4 operator* (const Vec4 &v) const;
        Mat4 operator* (const Mat4 &m) const;

        float values[16];
    };
}