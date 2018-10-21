#pragma once
#include <string>

namespace Core
{
    enum EventType
    {
        eNull,

        // engine
        eShutdown,

        // entities
        eCreateEntity,
        eUnloadEntities,

        // graphics
        eAddModelComponent,
        eAddCameraComponent,

        // input
        eControllerInput,

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
        EventType getType() const override;
    };
}