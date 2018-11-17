#pragma once
#include "Event.h"


namespace Engine
{
    class ShutdownEvent : public Event
    {
    public:
        EventType getType() const override { return eShutdown; }
    };
}