#include "../../Include/Math/Transform.h"


Math::Transform::Transform() : position(0.0f), rotation(Math::Vec3(0.0f)), scale(1.0f)
{}


Math::Transform::Transform(const Transform &t) :
    position(t.position), rotation(t.rotation), scale(t.scale)
{}


Math::Transform::Transform(Vec3 p, Vec3 r, Vec3 s) :
    position(p), rotation(r), scale(s)
{}


Math::Transform::Transform(Vec3 p, Quat r, Vec3 s) :
    position(p), rotation(r), scale(s)
{}


Math::Transform& Math::Transform::operator= (const Transform &t)
{
    position = t.position;
    rotation = t.rotation;
    scale = t.scale;
    return *this;
}


Math::Transform::~Transform()
{}