#pragma once
#include "Event.h"

namespace Engine
{
    class ControllerInputEvent : public Event
    {
    public:

        ControllerInputEvent();
        EventType getType() const override;

        float lHorizontal, lVertical;
        float rHorizontal, rVertical;
        bool pause, select;
    };
}