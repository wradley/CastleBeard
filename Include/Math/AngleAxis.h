#pragma once
#include "../../Include/Math/Vector.h"

namespace Math
{
    class AngleAxis
    {
    public:

        AngleAxis(float angle, const Vec3 &v);
        ~AngleAxis();

        void  setAngle(float a);
        void  setAxis(const Vec3 &v);
        float getAngle() const;
        Vec3  getAxis() const;

        Vec3 data;

    };
}
