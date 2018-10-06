#pragma once

namespace Math
{
    const float PI = 3.14159f;

    float ToDegrees(float radians)
    {
        return radians * (180.0f / PI);
    }

    float ToRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }
}