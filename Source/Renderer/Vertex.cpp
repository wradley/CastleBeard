#include "Vertex.h"


namespace Renderer
{
    Vertex::Vertex() :
        position(0.0f), normal(0.0f, 1.0f, 0.0f)
    {}


    Vertex::Vertex(const Vertex & v) :
        position(v.position), normal(v.normal)
    {}


    Vertex & Vertex::operator=(const Vertex & v)
    {
        position = v.position;
        normal = v.normal;
        return *this;
    }


    Vertex::~Vertex() {}


    bool Vertex::operator==(const Vertex & v)
    {
        return (position == v.position) && (normal == v.normal);
    }
}
