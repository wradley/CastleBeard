#pragma once
#include "../System.h"
#include "CBMath.h"


namespace Engine
{
    class PlayerControllerSystem : public System
    {
    public:
        
        PlayerControllerSystem();
        ~PlayerControllerSystem();

        void init(EventManager & em) override;
        void update(EventManager & em) override;
        void stop(EventManager & em) override;

    private:

        void handleEvents(EventManager & em);
        void onCreateEntity(const CreateEntityEvent *e);
        void onUnloadEntity(const UnloadEntityEvent *e);
        void onControllerInput(const ControllerInputEvent *e, EventManager &em);
        void onTransformEntity(const TransformEntityEvent *e);
        void onTransformEntities(const TransformEntitiesEvent *e);
        void onAddPlayerControllerComponent(const AddPlayerControllerComponentEvent *e);

        unsigned int _playerID;
        unsigned int _cameraID;
        Math::Transform _playerTransform;
        Math::Transform _cameraTransform;

        EventQueue _eventQueue;
    };
}
