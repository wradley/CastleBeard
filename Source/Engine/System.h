#pragma once
#include "CBEvent.h"

namespace Engine
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