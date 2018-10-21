#pragma once
#include "Vector.h"
#include "Quaternion.h"

namespace Math
{
    class Transform
    {
    public:

        Transform();
        Transform(const Transform &t);
        Transform(Vec3 position, Vec3 rotation, Vec3 scale);
        Transform(Vec3 position, Quat rotation, Vec3 scale);
        Transform& operator= (const Transform &t);
        ~Transform();

        Vec3 position;
        Quat rotation;
        Vec3 scale;

    };
}