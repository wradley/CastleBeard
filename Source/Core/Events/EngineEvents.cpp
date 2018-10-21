#include "../../../Include/Core/Events/EngineEvents.h"

Core::EventType Core::ShutdownEvent::getType() const
{
    return EventType::eShutdown;
}
