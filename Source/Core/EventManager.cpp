#include "../../Include/Core/EventManager.h"
#include "../../Include/Debug.h"

Core::EventManager::EventManager()
{}


Core::EventManager::~EventManager() 
{}


void Core::EventManager::send(std::shared_ptr<const Event> e, const EventQueue *Q)
{
    // enqueue event to every listener
    std::lock_guard<std::mutex> lock(_mutex);
    for (EventQueue *q : _listeners[e->getType()]) {
        if (Q == q) continue; // if same queue as sender, skip
        q->pushBack(e);
    }
}


void Core::EventManager::listenFor(EventType t, EventQueue *q) 
{
    // add listener to event type
    std::lock_guard<std::mutex> lock(_mutex);

    // make sure they don't get added twice
    for (EventQueue *queue : _listeners[t]) {
        if (queue == q) {
            DEBUG_LOG("Listened to same event more than once");
            return;
        }
    }

    _listeners[t].push_back(q);
}