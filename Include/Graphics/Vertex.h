#pragma once
#include "../Math/Vector.h"

namespace Graphics
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(const Vertex &v);
        Vertex& operator= (const Vertex &v);
        ~Vertex();

        Math::Vec3 position;
    };
}