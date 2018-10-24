#pragma once
#include "../Event.h"
#include "../../Math/Transform.h"

namespace Core
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

    class ModComponentEvent : public ComponentEvent
    {
    public:
        virtual EventType getType() const = 0;
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


    class ModCameraComponentEvent : public ModComponentEvent
    {
    public:

        EventType getType() const override;

        bool modNearPlane;
        float nearPlane;

        bool modFarPlane;
        float farPlane;

        bool modAspectRatio;
        float aspectRatio;

        bool modFieldOfView;
        float fieldOfView;

        bool modTransform;
        Math::Transform transform;
    };
    

    // ------------------------------------------------------- PLAYER CONTOLLER
    class AddPlayerControllerComponentEvent : public AddComponentEvent
    {
    public:
        EventType getType() const override;

        unsigned int cameraComponent;
        Math::Transform entityTransform;
    };
}