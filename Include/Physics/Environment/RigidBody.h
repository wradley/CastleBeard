#pragma once
#include "../../Math/Vector.h"
#include "../../Math/AngleAxis.h"
#include "../../Math/Quaternion.h"
#include "../../Math/Matrix.h"

namespace Physics
{
    class Rigidbody
    {
    public:

        Math::Vec3 acceleration;
        Math::Vec3 velocity;
        Math::Vec3 position;

        Math::Quat orientation;
        Math::AngleAxis angularVelocity;
        Math::AngleAxis torque;

        float inverseMass;
        Math::Mat3 inverseInertia;

    };
}