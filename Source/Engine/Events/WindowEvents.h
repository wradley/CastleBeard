#pragma once
#include "Event.h"

namespace Engine
{
    class ResizeWindowEvent : public Event
    {
    public:

        EventType getType() const override;

        unsigned int newWidth;
        unsigned int newHeight;
    };
}