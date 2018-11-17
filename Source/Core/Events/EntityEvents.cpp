#include "../../../Include/Core/Events/EntityEvents.h"


Core::EventType Core::CreateEntityEvent::getType() const
{
    return EventType::eCreateEntity;
}


Core::EventType Core::UnloadEntityEvent::getType() const
{
    return EventType::eUnloadEntity;
}


Core::EventType Core::TransformEntityEvent::getType() const
{
    return EventType::eTransformEntity;
}


Core::EventType Core::TransformEntitiesEvent::getType() const
{
    return EventType::eTransformEntities;
}