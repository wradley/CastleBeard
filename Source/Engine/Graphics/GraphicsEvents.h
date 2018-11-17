#pragma once
#include "../Events/ComponentEvents.h"

namespace Engine
{
    class AddModelComponentEvent : public Engine::AddComponentWithTransformEvent
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
}