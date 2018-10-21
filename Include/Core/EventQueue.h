#pragma once
#include <atomic>
#include <memory>
#include "Event.h"

// lockless & thread safe for SINGLE producer and SINGLE consumer

namespace Core
{
    class EventQueue
    {
    public:

        EventQueue();
        ~EventQueue();

        unsigned int size() const;
        void pushBack(const std::shared_ptr<const Event> &value);
        std::shared_ptr<const Event> popFront();

    private:

        struct Node
        {
            Node() : next(nullptr) {}
            Node *next;
            std::shared_ptr<const Event> data;
        };

        std::atomic_uint _size;
        Node *_front;
        Node *_back;

    private:

        EventQueue(const EventQueue &) = delete;
        EventQueue(EventQueue &&) = delete;
    };
}