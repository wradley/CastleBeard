#include "../../../Include/Core/Events/ComponentEvents.h"


// ------------------------------------------------------------------- GRAPHICS
Core::EventType Core::AddModelComponentEvent::getType() const
{
    return EventType::eAddModelComponent;
}


Core::EventType Core::AddCameraComponentEvent::getType() const
{
    return EventType::eAddCameraComponent;
}