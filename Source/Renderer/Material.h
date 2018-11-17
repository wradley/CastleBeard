#pragma once
#include "CBMath.h"

namespace Renderer
{
    class Material
    {
    public:

        unsigned int albedo;
        Math::Vec3 albedoColor;

        unsigned int normal;
        Math::Vec3 normalColor;

        unsigned int mra;
        Math::Vec3 mraColor;
    };
}