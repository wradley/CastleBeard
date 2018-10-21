#include "../../../Include/Core/Events/WindowEvents.h"


Core::EventType Core::ResizeWindowEvent::getType() const
{
    return EventType::eResizeWindow;
}