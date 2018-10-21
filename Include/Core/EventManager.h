#pragma once
#include <map>
#include <vector>
#include <mutex>
#include "EventQueue.h"

// event manager is thread safe class to pass events between listeners
// - because it is protected by a mutex, the queues will only have a 
//   single producer

namespace Core
{
    class EventManager
    {
    public:

        EventManager();
        ~EventManager();

        // must give provide your queue so it doesn't send you the event
        void send(std::shared_ptr<const Event> e, const EventQueue *q);

        // will populate your queue when it recieves an event match
        void listenFor(EventType t, EventQueue *q);

    private:

        std::mutex _mutex;
        std::map<EventType, std::vector<EventQueue*>> _listeners;
    };
}