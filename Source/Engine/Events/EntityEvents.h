#pragma once
#include <vector>
#include <memory>
#include "Event.h"
#include "CBMath.h"
#include "ComponentEvents.h"

namespace Engine
{
    class EntityEvent : public Event
    {
    public:
        virtual EventType getType() const = 0;
        unsigned int entity;
    };


    class CreateEntityEvent : public EntityEvent
    {
    public:

        EventType getType() const override { return eCreateEntity; }
        unsigned int parent;
        Math::Transform transform;
        std::vector<std::shared_ptr<const AddComponentEvent>> components;
    };


    // ** expected to unload all children of entity **
    class UnloadEntityEvent : public EntityEvent
    {
    public:
        EventType getType() const override { return eUnloadEntity; }
    };


    class TransformEntityEvent : public EntityEvent
    {
    public:
        EventType getType() const override { return eTransformEntity; }
        Math::Transform transform;
    };


    class TransformEntitiesEvent : public Event
    {
    public:
        EventType getType() const override { return eTransformEntities; }
        std::vector<std::shared_ptr<const TransformEntityEvent>> transformEvents;
    };
}