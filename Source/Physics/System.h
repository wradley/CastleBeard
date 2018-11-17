#pragma once
#include <chrono>
#include <map>
#include "Core/System.h"
#include "Core/EventManager.h"
#include "Core/Events/ComponentEvents.h"
#include "Core/Events/EntityEvents.h"
#include "PhysicsEnvironment.h"

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

        void handleEvents();
        void onEvent(std::shared_ptr<const Core::Event> e);
        void onCreateEntity(const Core::CreateEntityEvent *e);
        void onUnloadEntity(const Core::UnloadEntityEvent *e);
        void onTransformEntity(const Core::TransformEntityEvent *e);
        void onTransformEntities(const Core::TransformEntitiesEvent *e);
        void onAddRigidbodyComponent(const Core::AddRigidbodyComponentEvent *e);

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
            Rigidbody *rigidbody;
            Math::Transform transform;
        };

        std::vector<unsigned int> _entitiesMap;
        std::vector<PEnt> _physicsEnts;

        PhysicsEnvironment *_environment;
        Core::EventQueue _eventQueue;
        std::chrono::time_point<std::chrono::steady_clock> _lastTime;
    };
}