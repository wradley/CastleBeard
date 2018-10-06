#include "../../Include/Graphics/Vertex.h"


Graphics::Vertex::Vertex() : position(0.0f)
{}


Graphics::Vertex::Vertex(const Vertex & v) : position(v.position)
{}


Graphics::Vertex & Graphics::Vertex::operator=(const Vertex & v)
{
    position = v.position;
    return *this;
}


Graphics::Vertex::~Vertex() {}
