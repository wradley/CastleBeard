#pragma once
#include "../Events/ComponentEvents.h"

namespace Engine
{
    class AddPlayerControllerComponentEvent : public AddComponentWithTransformEvent
    {
    public:
        EventType getType() const override;

        unsigned int cameraComponent;
    };
}