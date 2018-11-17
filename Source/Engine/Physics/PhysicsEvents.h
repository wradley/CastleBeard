#pragma once
#include "../Events/ComponentEvents.h"

namespace Engine
{
    class AddRigidbodyComponentEvent : public Engine::AddComponentEvent
    {
    public:

        EventType getType() const override { return eAddRigidbodyComponent; }

        float invertedMass;
        Math::Vec3 acceleration;
        Math::Vec3 velocity;

        Math::Transform transform;
    };
}