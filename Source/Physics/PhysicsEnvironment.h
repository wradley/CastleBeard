#pragma once
#include <vector>
#include <deque>
#include "CBMath.h"
#include "RigidBody.h"

namespace Physics
{
    class PhysicsEnvironment
    {
    public:

        PhysicsEnvironment();
        ~PhysicsEnvironment();

        void update(float timeStep);
        
        Rigidbody* createRigidbody();
        void deleteRigidbody(Rigidbody *r);

    private:

        std::vector<Rigidbody*> _rigidBodies;

    };
}