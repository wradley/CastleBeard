#pragma once
#include <vector>
#include <tuple>
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


    // --------------------------------------------------------------- GRAPHICS
    class AddModelComponentEvent : public AddComponentWithTransformEvent
    {
    public:

        EventType getType() const override;

        std::string filepath;
    };

    class AddCameraComponentEvent : public AddComponentWithTransformEvent
    {
    public:

        EventType getType() const override;

        float nearPlane;
        float farPlane;
        float aspectRatio;
        float fieldOfView;
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

        bool modComponentTForm;
        Math::Transform componentTform;
    };
    

    // ------------------------------------------------------- PLAYER CONTOLLER
    class AddPlayerControllerComponentEvent : public AddComponentWithTransformEvent
    {
    public:
        EventType getType() const override;

        unsigned int cameraComponent;
    };
}