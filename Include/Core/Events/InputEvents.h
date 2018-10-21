#pragma once
#include "../Event.h"

namespace Core
{
    class ControllerInputEvent : public Event
    {
    public:

        ControllerInputEvent();
        EventType getType() const override;

        float lHorizontal, lVertical;
        float rHorizontal, rVertical;
    };
}