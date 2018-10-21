#pragma once
#include <map>
#include "../Core/System.h"
#include "Environment.h"
#include "../Core/Events/EntityEvents.h"
#include "../Core/Events/ComponentEvents.h"

namespace Graphics
{
    class System : public Core::System
    {
    public:

        System();
        ~System();

        void init(Core::EventManager & em) override;
        void update(Core::EventManager & em) override;
        void stop(Core::EventManager & em) override;

    private:

        void handleEvents();
        void onCreateEntityEvent(const Core::CreateEntityEvent *e);
        void onUnloadEntitiesEvent(const Core::UnloadEntitiesEvent *e);
        void onAddModelEvent(const Core::AddModelComponentEvent *e);
        void onAddCameraEvent(const Core::AddCameraComponentEvent *e);

        Core::EventQueue _eventQueue;
        Environment _environment;

        std::map<unsigned int, unsigned int> _entitiesToNodes;
    };
}