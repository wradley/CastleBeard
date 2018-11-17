#pragma once

namespace Math
{
    class Vec3;
    class Vec4;

    class Quat
    {
    public:

        Quat();
        Quat(const Vec3 &euler);
        Quat(const Vec4 &quat);
        Quat(const Quat &quat);
        Quat& operator= (const Quat &quat);
        Quat(float X, float Y, float Z, float W);
        ~Quat();

        static Quat FromEuler(const Vec3 &v);

        Quat operator* (const Quat &q) const;
        Vec3 operator* (const Vec3 &v) const;

        void normalize();

        float x, y, z, w;
    };

    Quat Normalize(const Quat &q);
    void HamiltonProduct(
        float Aw, float Ax, float Ay, float Az, 
        float Bw, float Bx, float By, float Bz,
        float &W, float &X, float &Y, float &Z
    );
}