#pragma once
#include "InputEvents.h"


namespace Engine
{
    ControllerInputEvent::ControllerInputEvent() :
        lHorizontal(0.0f), lVertical(0.0f),
        rHorizontal(0.0f), rVertical(0.0f),
        pause(false), select(false)
    {}


    EventType ControllerInputEvent::getType() const
    {
        return EventType::eControllerInput;
    }
}