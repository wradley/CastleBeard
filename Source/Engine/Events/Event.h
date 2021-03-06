#pragma once
#include <string>

namespace Engine
{
    enum EventType
    {
        eNull = 0,

        // engine
        eShutdown,

        // entities
        eCreateEntity,
        eUnloadEntity,
        eTransformEntity,
        eTransformEntities,

        // graphics
        eAddModelComponent,
        eAddCameraComponent,
        eModCameraComponent,

        // physics
        eAddRigidbodyComponent,

        // input
        eControllerInput,

        // player controller
        eAddPlayerControllerComponent,

        // window
        eResizeWindow
    };


    class Event
    {
    public:
        Event() {}
        virtual ~Event() {}
        virtual EventType getType() const = 0;
    private:
        Event(const Event &) = delete;
    };


    class NullEvent : public Event
    {
    public:
        NullEvent() {}
        ~NullEvent() {}
        EventType getType() const override { return eNull; }
    };
}