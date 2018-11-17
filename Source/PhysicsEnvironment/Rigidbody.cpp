#include "RigidBody.h"


namespace Physics
{
    Rigidbody::Rigidbody() : 
        _acceleration(0.0f), _velocity(0.0f), _position(0.0f), 
        _invertedMass(1.0f)
    {}


    Rigidbody::~Rigidbody()
    {}


    void Rigidbody::applyForce(const Math::Vec3 &force)
    {
        _acceleration += force * _invertedMass;
    }


    void Rigidbody::copyValuesOf(const Rigidbody &r)
    {
        _acceleration = r._acceleration;
        _velocity = r._velocity;
        _position = r._position;
        _invertedMass = r._invertedMass;
    }


    void Rigidbody::setAcceleration(const Math::Vec3 &a)
    {
        _acceleration = a;
    }


    void Rigidbody::setVelocity(const Math::Vec3 &v)
    {
        _velocity = v;
    }


    void Rigidbody::setPosition(const Math::Vec3 &p)
    {
        _position = p;
    }


    void Rigidbody::setInvertedMass(float m)
    {
        _invertedMass = m;
    }


    void Rigidbody::setMass(float m)
    {
        _invertedMass = 1.0f / m;
    }


    Math::Vec3 Rigidbody::getAcceleration() const
    {
        return _acceleration;
    }


    Math::Vec3 Rigidbody::getVelocity() const
    {
        return _velocity;
    }


    Math::Vec3 Rigidbody::getPosition() const
    {
        return _position;
    }


    float Rigidbody::getInvertedMass()
    {
        return _invertedMass;
    }


    float Rigidbody::getMass()
    {
        return 1.0f / _invertedMass;
    }


    void Rigidbody::integrate(float dt)
    {
        _velocity += _acceleration * dt;
        _position += _velocity * dt;
    }
}