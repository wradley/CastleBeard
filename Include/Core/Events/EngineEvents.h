#pragma once
#include "../Event.h"


namespace Core
{
    // --------------------------------------------------------------- ENTITIES
    class ShutdownEvent : public Event
    {
    public:
        EventType getType() const override;
    };
}