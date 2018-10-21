#pragma once
#include "../../Math/Vector.h"

namespace Graphics
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(const Vertex &v);
        Vertex& operator= (const Vertex &v);
        ~Vertex();

        bool operator== (const Vertex &v);

        Math::Vec3 position;
        Math::Vec3 normal;
    };
}