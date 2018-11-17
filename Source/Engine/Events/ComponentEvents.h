#pragma once
#include <vector>
#include <tuple>
#include "Event.h"
#include "CBMath.h"

namespace Engine
{
    class ComponentEvent : public Event
    {
    public:
        virtual EventType getType() const = 0;
        unsigned int component;
    };


    class AddComponentEvent : public ComponentEvent
    {
    public:
        virtual EventType getType() const = 0;
        unsigned int entity;
    };


    class AddComponentWithTransformEvent : public AddComponentEvent
    {
    public:
        virtual EventType getType() const = 0;
        Math::Transform entityTform;
        Math::Transform componentTform;

        // eldest from front to back, does not include new entity
        std::vector<std::tuple<unsigned int, Math::Transform>> lineage;
    };

    class ModComponentEvent : public ComponentEvent
    {
    public:
        virtual EventType getType() const = 0;
    };
}