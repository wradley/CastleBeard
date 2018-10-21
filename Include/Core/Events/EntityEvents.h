#pragma once
#include <vector>
#include "../Event.h"
#include "../../Math/Transform.h"

namespace Core
{
    // --------------------------------------------------------------- ENTITIES
    class CreateEntityEvent : public Event
    {
    public:
        EventType getType() const override;
        unsigned int entity;
        unsigned int parent;
        Math::Transform transform;
    };


    // ** expected to unload all children of entity **
    class UnloadEntitiesEvent : public Event
    {
    public:
        EventType getType() const override;
        std::vector<unsigned int> entities;
    };
}