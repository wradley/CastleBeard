#pragma once
#include "CBMath.h"

namespace Physics
{
    class Rigidbody
    {
    public:

        Rigidbody();
        ~Rigidbody();

        void applyForce(const Math::Vec3 &force);

        void copyValuesOf(const Rigidbody &r);
        void setAcceleration(const Math::Vec3 &a);
        void setVelocity(const Math::Vec3 &v);
        void setPosition(const Math::Vec3 &p);
        void setInvertedMass(float m);
        void setMass(float m);

        Math::Vec3 getAcceleration() const;
        Math::Vec3 getVelocity() const;
        Math::Vec3 getPosition() const;
        float getInvertedMass();
        float getMass();

    private:

        friend class PhysicsEnvironment;
        void integrate(float deltaTime);

    private:

        Math::Vec3 _acceleration;
        Math::Vec3 _velocity;
        Math::Vec3 _position;

        float _invertedMass;

    };
}