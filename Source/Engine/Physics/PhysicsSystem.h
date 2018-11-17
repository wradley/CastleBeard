#pragma once
#include <chrono>
#include <map>
#include "../System.h"
#include "PhysicsEnvironment.h"

namespace Engine
{
    class PhysicsSystem : public System
    {
    public:

        PhysicsSystem();
        ~PhysicsSystem();

        void init(EventManager &em) override;
        void update(EventManager &em) override;
        void stop(EventManager &em) override;

    private:

        void handleEvents();
        void onEvent(std::shared_ptr<const Event> e);
        void onCreateEntity(const CreateEntityEvent *e);
        void onUnloadEntity(const UnloadEntityEvent *e);
        void onTransformEntity(const TransformEntityEvent *e);
        void onTransformEntities(const TransformEntitiesEvent *e);
        void onAddRigidbodyComponent(const AddRigidbodyComponentEvent *e);

        bool haveEntity(unsigned int ent) const;

    private:

        struct PEnt
        {
            PEnt(unsigned int entityID, unsigned int componentID);
            PEnt(const PEnt &p);
            ~PEnt();

            void updateTransformFromRigidbody();

            unsigned int entityID;
            unsigned int componentID;
            Physics::Rigidbody *rigidbody;
            Math::Transform transform;
        };

        std::vector<unsigned int> _entitiesMap;
        std::vector<PEnt> _physicsEnts;

        Physics::PhysicsEnvironment *_environment;
        EventQueue _eventQueue;
        std::chrono::time_point<std::chrono::steady_clock> _lastTime;
    };
}