#include "AngleAxis.h"


Math::AngleAxis::AngleAxis(float angle, const Vec3 &v)
{
    data = Math::Normalize(v) * angle;
}


Math::AngleAxis::~AngleAxis()
{}


void Math::AngleAxis::setAngle(float a)
{
    data = Math::Normalize(data) * a;
}


void Math::AngleAxis::setAxis(const Vec3 &v)
{
    float len = Math::Length(data);
    data = Math::Normalize(v) * len;
}


float Math::AngleAxis::getAngle() const
{
    return Math::Length(data);
}


Math::Vec3 Math::AngleAxis::getAxis() const
{
    return Math::Normalize(data);
}