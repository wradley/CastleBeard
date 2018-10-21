#pragma once
#include "../Event.h"
#include "../../Math/Transform.h"

namespace Core
{
    class AddComponentEvent : public Event
    {
    public:
        virtual EventType getType() const = 0;
        unsigned int entity;
    };


    // --------------------------------------------------------------- GRAPHICS
    class AddModelComponentEvent : public AddComponentEvent
    {
    public:

        EventType getType() const override;

        std::string filepath;
        Math::Transform transform;
    };

    class AddCameraComponentEvent : public AddComponentEvent
    {
    public:

        EventType getType() const override;

        float nearPlane;
        float farPlane;
        float aspectRatio;
        float fieldOfView;
        Math::Transform transform;
    };
}