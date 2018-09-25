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
        Quat(const Vec4 &vec);
        Quat(const Quat &quat);
        Quat& operator= (const Quat &quat);
        Quat(float X, float Y, float Z, float W);
        ~Quat();

        static Quat FromEuler(const Vec3 &v);

        Quat operator* (const Quat &q) const;

        void Normalize();

        float x, y, z, w;
    };

    Quat Normalize(const Quat &q);
}