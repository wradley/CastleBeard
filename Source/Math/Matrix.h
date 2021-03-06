#pragma once

namespace Math
{
    class Quat;
    class Vec3;
    class Vec4;
    class Transform;

    //class Mat3
    //{
    //public:

    //    Mat3();
    //    Mat3(float i);
    //    Mat3(const float(&v)[9]);
    //    Mat3(const Mat3 &m);
    //    Mat3(const Quat &q);
    //    Mat3& operator= (const Mat3 &m);
    //    ~Mat3();

    //    static Mat4 FromQuat(const Quat &q);

    //    Mat3 operator* (float s) const;
    //    Vec3 operator* (const Vec3 &v) const;
    //    Mat3 operator* (const Mat3 &m) const;

    //    float values[9];
    //};

    class Mat4
    {
    public:

        Mat4();
        Mat4(float i);
        Mat4(const float(&v)[16]);
        Mat4(const Mat4 &m);
        Mat4(const Transform &t);
        Mat4(const Quat &q);
        Mat4& operator= (const Mat4 &m);
        ~Mat4();

        static Mat4 FromQuat(const Quat &q);

        Mat4 operator* (float s) const;
        Vec4 operator* (const Vec4 &v) const;
        Mat4 operator* (const Mat4 &m) const;

        float values[16];
    };

    Mat4 Scale(const Mat4 &m, const Vec3 &v);
    Mat4 Transpose(const Mat4 &m, const Vec3 &v);
    Mat4 Perspective(float fov, float aspect, float nearplane, float farplane);
    Mat4 LookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up);
}
