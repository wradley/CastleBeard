#pragma once
#include "../Core/System.h"
#include "../Core/EventManager.h"
#include "../Core/Events/ComponentEvents.h"
#include "../Core/Events/EntityEvents.h"

namespace Physics
{
    class System : public Core::System
    {
    public:

        System();
        ~System();

        void init(Core::EventManager &em) override;
        void update(Core::EventManager &em) override;
        void stop(Core::EventManager &em) override;

    private:

        /*void handleEvents();
        void onEvent(std::shared_ptr<const Core::Event> e);
        void onCreateEntity(const Core::CreateEntityEvent *e);
        void onUnloadEntity(const Core::UnloadEntityEvent *e);
        void onAddModel(const Core::AddModelComponentEvent *e);
        void onAddCamera(const Core::AddCameraComponentEvent *e);
        void onModCamera(const Core::ModCameraComponentEvent *e);
        void onTransformEntity(const Core::TransformEntityEvent *e);*/

        Core::EventQueue _eventQueue;
    };
}