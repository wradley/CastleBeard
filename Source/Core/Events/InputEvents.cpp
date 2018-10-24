#pragma once
#include "../../../Include/Core/Events/InputEvents.h"


Core::ControllerInputEvent::ControllerInputEvent() :
    lHorizontal(0.0f), lVertical(0.0f),
    rHorizontal(0.0f), rVertical(0.0f),
    pause(false), select(false)
{}


Core::EventType Core::ControllerInputEvent::getType() const
{
    return EventType::eControllerInput;
}