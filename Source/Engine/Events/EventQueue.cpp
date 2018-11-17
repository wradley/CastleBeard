#include "EventQueue.h"

// empty queue
// [FRONT/BACK]->NULL (size = 0)

// add to empty queue
// [FRONT/BACK]->NULL                (size = 0)
// [  FRONT   ]->[  BACK   ]->NULL   (size = 0)
// [  FRONT   ]->[  BACK   ]->NULL   (size = 1)

// add to filled queue
// [ FRONT ]->[ ]->...->[ ]->[ BACK=n ]->NULL                      (size = n)
// [ FRONT ]->[ ]->...->[ ]->[   n    ]->[ BACK=n+1 ]->NULL        (size = n)
// [ FRONT ]->[ ]->...->[ ]->[   n    ]->[ BACK=n+1 ]->NULL        (size = n+1)

namespace Engine
{
    EventQueue::EventQueue() : _size(0), _front(nullptr), _back(nullptr)
    {
        // create a dummy node
        Node *dummy = new Node;
        _front = dummy;
        _back = dummy;
    }


    EventQueue::~EventQueue()
    {
        while (_front->next != nullptr) {
            Node *old = _front;
            _front = _front->next;
            delete old;
        }
    }


    unsigned int EventQueue::size() const
    {
        return _size.load();
    }


    void EventQueue::pushBack(const std::shared_ptr<const Event> &value)
    {
        // fill in data of old dummy
        _back->data = value;

        // create new dummy
        Node *dummy = new Node;

        // point back to dummy
        _back->next = dummy;

        // set dummy as back
        _back = dummy;

        // inc size
        ++_size;
    }


    std::shared_ptr<const Event> EventQueue::popFront()
    {
        // if there exists at least one node
        if (_size.load() > 0) {

            // dec the size
            --_size;

            // save the data
            std::shared_ptr<const Event> data = _front->data;

            // point the front to the next node
            Node *old = _front;
            _front = _front->next;

            // delete the node
            delete old;

            return data;
        }

        else {
            return std::shared_ptr<const Event>(new NullEvent);
        }
    }
}