#pragma once
#include "Core/System.h"
#include "Core/EventQueue.h"
#include "Core/Events/InputEvents.h"
#include "Core/Events/EntityEvents.h"
#include "Core/Events/ComponentEvents.h"
#include "CBMath.h"


namespace Engine
{
    class PlayerControllerSystem : public Core::System
    {
    public:
        
        PlayerControllerSystem();
        ~PlayerControllerSystem();

        void init(Core::EventManager & em) override;
        void update(Core::EventManager & em) override;
        void stop(Core::EventManager & em) override;

    private:

        void handleEvents(Core::EventManager & em);
        void onCreateEntity(const Core::CreateEntityEvent *e);
        void onUnloadEntity(const Core::UnloadEntityEvent *e);
        void onControllerInput(const Core::ControllerInputEvent *e, Core::EventManager &em);
        void onTransformEntity(const Core::TransformEntityEvent *e);
        void onTransformEntities(const Core::TransformEntitiesEvent *e);
        void onAddPlayerControllerComponent(const Core::AddPlayerControllerComponentEvent *e);

        unsigned int _playerID;
        unsigned int _cameraID;
        Math::Transform _playerTransform;
        Math::Transform _cameraTransform;

        Core::EventQueue _eventQueue;
    };
}
