#include "WindowEvents.h"


namespace Engine
{
    EventType ResizeWindowEvent::getType() const
    {
        return EventType::eResizeWindow;
    }
}