#include "../../../Include/Core/Events/EntityEvents.h"


Core::EventType Core::CreateEntityEvent::getType() const
{
    return EventType::eCreateEntity;
}


Core::EventType Core::UnloadEntitiesEvent::getType() const
{
    return EventType::eUnloadEntities;
}