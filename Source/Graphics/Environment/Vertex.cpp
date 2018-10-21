#include "../../../Include/Graphics/Environment/Vertex.h"


Graphics::Vertex::Vertex() : 
    position(0.0f), normal(0.0f, 1.0f, 0.0f)
{}


Graphics::Vertex::Vertex(const Vertex & v) : 
    position(v.position), normal(v.normal)
{}


Graphics::Vertex & Graphics::Vertex::operator=(const Vertex & v)
{
    position = v.position;
    normal = v.normal;
    return *this;
}


Graphics::Vertex::~Vertex() {}


bool Graphics::Vertex::operator==(const Vertex & v)
{
    return (position == v.position) && (normal == v.normal);
}
