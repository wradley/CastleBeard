#include "../../Include/Math/Math.h"

float Math::ToDegrees(float radians)
{
    return radians * (180.0f / PI);
}

float Math::ToRadians(float degrees)
{
    return degrees * (PI / 180.0f);
}