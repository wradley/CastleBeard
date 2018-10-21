#pragma once
#include "EventManager.h"

namespace Core
{
    class System
    {
    public:

        virtual void init(EventManager &em) = 0;
        virtual void update(EventManager &em) = 0;
        virtual void stop(EventManager &em) = 0;

    private:
    };
}