#pragma once
#include "../Event.h"

namespace Core
{
    class ResizeWindowEvent : public Event
    {
    public:

        EventType getType() const override;

        unsigned int newWidth;
        unsigned int newHeight;
    };
}