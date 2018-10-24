#pragma once
#include <vector>
#include "../Event.h"
#include "../../Math/Transform.h"
#include "ComponentEvents.h"

namespace Core
{
    class EntityEvent : public Event
    {
    public:
        virtual EventType getType() const = 0;
        unsigned int entity;
    };

    // --------------------------------------------------------------- ENTITIES
    class CreateEntityEvent : public EntityEvent
    {
    public:
        EventType getType() const override;
        unsigned int parent;
        Math::Transform transform;
        std::vector<std::shared_ptr<const AddComponentEvent>> components;
    };


    // ** expected to unload all children of entity **
    class UnloadEntityEvent : public EntityEvent
    {
    public:
        EventType getType() const override;
    };


    class TransformEntityEvent : public EntityEvent
    {
    public:
        EventType getType() const override;
        Math::Transform transform;
    };
}