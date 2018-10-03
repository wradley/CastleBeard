#include "Transform.h"


gp::Transform::Transform() : position(0.0f), rotation(glm::vec3(0.0f)), scale(1.0f)
{}


gp::Transform::Transform(const Transform &t) :
    position(t.position), rotation(t.rotation), scale(t.scale)
{}


gp::Transform& gp::Transform::operator= (const Transform &t)
{
    position = t.position;
    rotation = t.rotation;
    scale = t.scale;
    return *this;
}


gp::Transform::~Transform()
{}


glm::mat4 gp::Transform::genMatrix()
{
    glm::mat4 i(1.0f);
    glm::mat4 p = glm::translate(i, position);
    glm::mat4 r = glm::toMat4(rotation);
    glm::mat4 s = glm::scale(i, scale);
    return p * r * s;
}
