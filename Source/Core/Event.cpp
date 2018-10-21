#include "../../Include/Core/Event.h"


Core::EventType Core::NullEvent::getType() const
{
    return EventType::eNull;
}