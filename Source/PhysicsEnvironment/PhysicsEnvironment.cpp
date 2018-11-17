#include "PhysicsEnvironment.h"
#include "Debug.h"


namespace Physics
{
    PhysicsEnvironment::PhysicsEnvironment()
    {}


    PhysicsEnvironment::~PhysicsEnvironment()
    {
        for (auto rb : _rigidBodies) delete rb;
    }


    void PhysicsEnvironment::update(float dt)
    {
        for (auto rb : _rigidBodies) {
            rb->integrate(dt);
            rb->setAcceleration(Math::Vec3(0.0f, -9.8f, 0.0f));
        }
    }


    Rigidbody* PhysicsEnvironment::createRigidbody()
    {
        Rigidbody *rb = new Rigidbody;
        _rigidBodies.push_back(rb);
        return rb;
    }


    void PhysicsEnvironment::deleteRigidbody(Rigidbody *r)
    {
        // copy back to this location and pop back
        unsigned int size = (unsigned int) _rigidBodies.size();
        for (unsigned int i = 0; i < size; ++i) {
            if (_rigidBodies[i] == r) {
                _rigidBodies[i] = _rigidBodies.back();
                _rigidBodies.pop_back();
                break;
            }
        }

        delete r;
    }
}