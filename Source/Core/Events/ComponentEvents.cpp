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


Core::EventType Core::ModCameraComponentEvent::getType() const
{
    return EventType::eModCameraComponent;
}


// ---------------------------------------------------------- PLAYER CONTROLLER
Core::EventType Core::AddPlayerControllerComponentEvent::getType() const
{
    return EventType::eAddPlayerControllerComponent;
}


// -------------------------------------------------------------------- PHYSICS
Core::EventType Core::AddRigidbodyComponentEvent::getType() const
{
    return EventType::eAddRigidbodyComponent;
}