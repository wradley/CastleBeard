#include "PhysicsSystem.h"
#include "CBMath.h"
#include "Debug.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {}


    PhysicsSystem::~PhysicsSystem()
    {}


    void PhysicsSystem::init(Core::EventManager &em)
    {
        _environment = new Physics::PhysicsEnvironment;
        _lastTime = std::chrono::high_resolution_clock::now();

        em.listenFor(Core::EventType::eCreateEntity, &_eventQueue);
        em.listenFor(Core::EventType::eUnloadEntity, &_eventQueue);
        em.listenFor(Core::EventType::eTransformEntities, &_eventQueue);
        em.listenFor(Core::EventType::eTransformEntity, &_eventQueue);
        em.listenFor(Core::EventType::eAddRigidbodyComponent, &_eventQueue);
    }


    void PhysicsSystem::update(Core::EventManager &em)
    {
        handleEvents();

        // step environment
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = now - _lastTime;
        _environment->update(delta.count());
        _lastTime = now;

        // send out new transform data
        Core::TransformEntitiesEvent *tformEntities = new Core::TransformEntitiesEvent;

        for (auto pEnt : _physicsEnts) {
            pEnt.updateTransformFromRigidbody();

            // add transform to list
            auto te = new Core::TransformEntityEvent;
            te->entity = pEnt.entityID;
            te->transform = pEnt.transform;
            tformEntities->transformEvents.push_back(
                std::shared_ptr<const Core::TransformEntityEvent>(te)
            );
        }

        em.send(
            std::shared_ptr<const Core::TransformEntitiesEvent>(tformEntities),
            &_eventQueue
        );
    }


    void PhysicsSystem::stop(Core::EventManager &em)
    {
        delete _environment;
    }


    void PhysicsSystem::handleEvents()
    {
        unsigned int numEvents = _eventQueue.size();
        for (unsigned int i = 0; i < numEvents; ++i) {
            onEvent(_eventQueue.popFront());
        }
    }


    void PhysicsSystem::onEvent(std::shared_ptr<const Core::Event> e)
    {
        switch (e->getType())
        {
        case Core::EventType::eCreateEntity:
            onCreateEntity((Core::CreateEntityEvent*) e.get());
            break;
        case Core::EventType::eUnloadEntity:
            onUnloadEntity((Core::UnloadEntityEvent*) e.get());
            break;
        case Core::EventType::eTransformEntity:
            onTransformEntity((Core::TransformEntityEvent*) e.get());
            break;
        case Core::EventType::eTransformEntities:
            onTransformEntities((Core::TransformEntitiesEvent*) e.get());
            break;
        case Core::EventType::eAddRigidbodyComponent:
            onAddRigidbodyComponent((Core::AddRigidbodyComponentEvent*) e.get());
            break;
        default:
            break;
        }
    }


    void PhysicsSystem::onCreateEntity(const Core::CreateEntityEvent *e)
    {
        for (auto component : e->components) {
            if (component->getType() == Core::EventType::eAddRigidbodyComponent)
                onAddRigidbodyComponent((const Core::AddRigidbodyComponentEvent*)component.get());
        }
    }


    void PhysicsSystem::onUnloadEntity(const Core::UnloadEntityEvent *e)
    {
        if (haveEntity(e->entity)) {
            auto pEntIndex = _entitiesMap[e->entity];
            auto &ent = _physicsEnts[pEntIndex];
            _environment->deleteRigidbody(ent.rigidbody);

            // replace removed PEnt with back
            if (pEntIndex != _physicsEnts.size() - 1) {
                _physicsEnts[pEntIndex] = _physicsEnts.back();
                _physicsEnts.pop_back();
                _entitiesMap[_physicsEnts[pEntIndex].entityID] = pEntIndex;
            }

            _entitiesMap[e->entity] = 0;
        }
    }


    void PhysicsSystem::onTransformEntity(const Core::TransformEntityEvent *e)
    {
        // check that we have the entity (ie it has a physics component)
        if (haveEntity(e->entity)) {
            auto &ent = _physicsEnts[_entitiesMap[e->entity]];
            ent.transform = e->transform;
            ent.rigidbody->setPosition(ent.transform.position);
        }
    }


    void PhysicsSystem::onTransformEntities(const Core::TransformEntitiesEvent *e)
    {
        for (auto te : e->transformEvents) {
            onTransformEntity(te.get());
        }
    }


    void PhysicsSystem::onAddRigidbodyComponent(const Core::AddRigidbodyComponentEvent *e)
    {
        if (haveEntity(e->entity)) {
            DEBUG_LOG("Tried to add multiple rigid bodies to single entity");
            return;
        }

        Physics::Rigidbody *rb = _environment->createRigidbody();
        rb->setAcceleration(e->acceleration);
        rb->setInvertedMass(e->invertedMass);
        rb->setPosition(e->transform.position);
        rb->setVelocity(e->velocity);

        _physicsEnts.push_back(PEnt(e->entity, e->component));
        _physicsEnts.back().transform = e->transform;
        _physicsEnts.back().rigidbody = rb;

        _entitiesMap.resize(e->entity + 1, 0);
        _entitiesMap[e->entity] = _physicsEnts.size() - 1;
    }


    bool PhysicsSystem::haveEntity(unsigned int ent) const
    {
        if (ent < _entitiesMap.size() && _entitiesMap[ent] != 0) return true;
        return false;
    }


    PhysicsSystem::PEnt::PEnt(unsigned int EntityID, unsigned int ComponentID) :
        entityID(EntityID), componentID(ComponentID), rigidbody(nullptr)
    {}


    PhysicsSystem::PEnt::PEnt(const PEnt &p) :
        entityID(p.entityID), componentID(p.componentID),
        rigidbody(p.rigidbody), transform(p.transform)
    {}


    PhysicsSystem::PEnt::~PEnt()
    {}


    void PhysicsSystem::PEnt::updateTransformFromRigidbody()
    {
        transform.position = rigidbody->getPosition();
    }
}

