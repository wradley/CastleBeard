#include "../../Include/Physics/PhysicsEnvironment.h"
#include "../../Include/Debug.h"

Physics::PhysicsEnvironment::PhysicsEnvironment()
{}


Physics::PhysicsEnvironment::~PhysicsEnvironment()
{}


void Physics::PhysicsEnvironment::update(float dt)
{
    for (auto &node : _nodes) {
        node.lVel += node.lAccel * dt;
        node.lPos += node.lVel * dt;
    }
}


unsigned int Physics::PhysicsEnvironment::createNode()
{
    unsigned int newNode;

    if (_recycledNodeIDs.size()) {
        newNode = _recycledNodeIDs.front();
        _recycledNodeIDs.pop_front();
    } else {
        newNode = _nodes.size();
    }

    _nodes.resize(_nodes.size());
    _nodes[newNode].inverseMass = 1.0f;
    _nodes[newNode].valid = true;
}


void Physics::PhysicsEnvironment::removeNode(unsigned int node)
{
    if (node >= _nodes.size()) {
        DEBUG_LOG("Node out of bounds: " + std::to_string(node));
        return;
    }

    _nodes[node].valid = false;
}


void Physics::PhysicsEnvironment::applyForce(unsigned int node, const Math::Vec3 &force)
{}