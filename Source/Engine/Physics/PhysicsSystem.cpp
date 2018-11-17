#include "PhysicsSystem.h"
#include "CBMath.h"
#include "Debug.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {}


    PhysicsSystem::~PhysicsSystem()
    {}


    void PhysicsSystem::init(EventManager &em)
    {
        _environment = new Physics::PhysicsEnvironment;
        _lastTime = std::chrono::high_resolution_clock::now();

        em.listenFor(EventType::eCreateEntity, &_eventQueue);
        em.listenFor(EventType::eUnloadEntity, &_eventQueue);
        em.listenFor(EventType::eTransformEntities, &_eventQueue);
        em.listenFor(EventType::eTransformEntity, &_eventQueue);
        em.listenFor(EventType::eAddRigidbodyComponent, &_eventQueue);
    }


    void PhysicsSystem::update(EventManager &em)
    {
        handleEvents();

        // step environment
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = now - _lastTime;
        _environment->update(delta.count());
        _lastTime = now;

        // send out new transform data
        TransformEntitiesEvent *tformEntities = new TransformEntitiesEvent;

        for (auto pEnt : _physicsEnts) {
            pEnt.updateTransformFromRigidbody();

            // add transform to list
            auto te = new TransformEntityEvent;
            te->entity = pEnt.entityID;
            te->transform = pEnt.transform;
            tformEntities->transformEvents.push_back(
                std::shared_ptr<const TransformEntityEvent>(te)
            );
        }

        em.send(
            std::shared_ptr<const TransformEntitiesEvent>(tformEntities),
            &_eventQueue
        );
    }


    void PhysicsSystem::stop(EventManager &em)
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


    void PhysicsSystem::onEvent(std::shared_ptr<const Event> e)
    {
        switch (e->getType())
        {
        case EventType::eCreateEntity:
            onCreateEntity((CreateEntityEvent*) e.get());
            break;
        case EventType::eUnloadEntity:
            onUnloadEntity((UnloadEntityEvent*) e.get());
            break;
        case EventType::eTransformEntity:
            onTransformEntity((TransformEntityEvent*) e.get());
            break;
        case EventType::eTransformEntities:
            onTransformEntities((TransformEntitiesEvent*) e.get());
            break;
        case EventType::eAddRigidbodyComponent:
            onAddRigidbodyComponent((AddRigidbodyComponentEvent*) e.get());
            break;
        default:
            break;
        }
    }


    void PhysicsSystem::onCreateEntity(const CreateEntityEvent *e)
    {
        for (auto component : e->components) {
            if (component->getType() == EventType::eAddRigidbodyComponent)
                onAddRigidbodyComponent((const AddRigidbodyComponentEvent*)component.get());
        }
    }


    void PhysicsSystem::onUnloadEntity(const UnloadEntityEvent *e)
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


    void PhysicsSystem::onTransformEntity(const TransformEntityEvent *e)
    {
        // check that we have the entity (ie it has a physics component)
        if (haveEntity(e->entity)) {
            auto &ent = _physicsEnts[_entitiesMap[e->entity]];
            ent.transform = e->transform;
            ent.rigidbody->setPosition(ent.transform.position);
        }
    }


    void PhysicsSystem::onTransformEntities(const TransformEntitiesEvent *e)
    {
        for (auto te : e->transformEvents) {
            onTransformEntity(te.get());
        }
    }


    void PhysicsSystem::onAddRigidbodyComponent(const AddRigidbodyComponentEvent *e)
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

