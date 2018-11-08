#pragma once
#include <vector>
#include <deque>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace Physics
{
    class PhysicsEnvironment
    {
    public:

        PhysicsEnvironment();
        ~PhysicsEnvironment();

        void update(float timeStep);
        
        unsigned int createNode();
        void removeNode(unsigned int node);
        void applyForce(unsigned int node, const Math::Vec3 &force);

    private:

        std::vector<Node> _nodes;
        std::deque<unsigned int> _recycledNodeIDs;
    };
}