#include "Object.h"


void gp::Object::setPosition(float x, float y, float z)
{
    _transform.position.x = x;
    _transform.position.y = y;
    _transform.position.z = z;
}


void gp::Object::setRotation(float x, float y, float z)
{
    _transform.rotation = glm::quat(glm::vec3(x, y, z));
}


void gp::Object::setRotationQuat(float x, float y, float z, float w)
{
    _transform.rotation.x = x;
    _transform.rotation.y = y;
    _transform.rotation.z = z;
    _transform.rotation.w = w;
}


void gp::Object::setScale(float x, float y, float z)
{
    _transform.scale.x = x;
    _transform.scale.y = y;
    _transform.scale.z = z;
}


