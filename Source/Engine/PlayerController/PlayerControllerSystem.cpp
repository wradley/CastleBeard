#include "PlayerControllerSystem.h"
#include "CBMath.h"
#include "Debug.h"


namespace Engine
{
    PlayerControllerSystem::PlayerControllerSystem() : _playerID(0)
    {}


    PlayerControllerSystem::~PlayerControllerSystem()
    {}


    void PlayerControllerSystem::init(EventManager & em)
    {
        em.listenFor(EventType::eCreateEntity, &_eventQueue);
        em.listenFor(EventType::eUnloadEntity, &_eventQueue);
        em.listenFor(EventType::eControllerInput, &_eventQueue);
        em.listenFor(EventType::eTransformEntity, &_eventQueue);
        em.listenFor(EventType::eTransformEntities, &_eventQueue);
        em.listenFor(EventType::eAddPlayerControllerComponent, &_eventQueue);
    }


    void PlayerControllerSystem::update(EventManager & em)
    {
        handleEvents(em);
    }


    void PlayerControllerSystem::stop(EventManager & em)
    {
        handleEvents(em);
    }


    void PlayerControllerSystem::handleEvents(EventManager & em)
    {
        unsigned int size = _eventQueue.size();
        for (unsigned int i = 0; i < size; ++i) {
            auto e = _eventQueue.popFront();
            switch (e->getType()) {
            case EventType::eCreateEntity:
                onCreateEntity((CreateEntityEvent*)e.get());
                break;
            case EventType::eControllerInput:
                onControllerInput((ControllerInputEvent*)e.get(), em);
                break;
            case EventType::eTransformEntity:
                onTransformEntity((TransformEntityEvent*)e.get());
                break;
            case EventType::eTransformEntities:
                onTransformEntities((TransformEntitiesEvent*)e.get());
                break;
            case EventType::eAddPlayerControllerComponent:
                onAddPlayerControllerComponent((AddPlayerControllerComponentEvent*)e.get());
                break;
            }
        }
    }


    void PlayerControllerSystem::onCreateEntity(const CreateEntityEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        for (auto ac : e->components) {
            if (ac->getType() == EventType::eAddPlayerControllerComponent) {
                onAddPlayerControllerComponent((const AddPlayerControllerComponentEvent*) ac.get());
            }
        }
    }


    void PlayerControllerSystem::onUnloadEntity(const UnloadEntityEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        if (e->entity == _playerID) _playerID = 0;
    }


    void PlayerControllerSystem::onControllerInput(const ControllerInputEvent *e, EventManager &em)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        if (_playerID) {

            // rotation
            float yaw = -e->lHorizontal;
            float pitch = e->lVertical;

            if (pitch > 89.0f) pitch = 89.0f; // clamp pitch
            if (pitch < -89.0f) pitch = -89.0f;

            Math::Quat yAxis(Math::Vec3(0.0f, Math::ToRadians(yaw), 0.0f));
            Math::Quat xAxis(Math::Vec3(Math::ToRadians(pitch), 0.0f, 0.0f));

            _playerTransform.orientation = _playerTransform.orientation * yAxis;
            _playerTransform.orientation.normalize();
            //std::cout << _playerTransform.rotation.x << " " << _playerTransform.rotation.y << " " << _playerTransform.rotation.z << " " << _playerTransform.rotation.w << std::endl;
            _cameraTransform.orientation = _cameraTransform.orientation * xAxis;
            _cameraTransform.orientation.normalize();

            // position
            //std::cout << e->rHorizontal << " " << e->rVertical << std::endl;
            Math::Vec3 dir(e->rHorizontal, 0.0f, -e->rVertical);
            dir = _playerTransform.orientation * _cameraTransform.orientation * dir;
            if (Math::Length(dir) > 0.0f)
                dir = Math::Normalize(dir);
            _playerTransform.position += dir / 2.0f;

            TransformEntityEvent *te = new TransformEntityEvent;
            te->entity = _playerID;
            te->transform = _playerTransform;
            em.send(std::shared_ptr<const TransformEntityEvent>(te), &_eventQueue);

            ModCameraComponentEvent *mc = new ModCameraComponentEvent;
            mc->modAspectRatio = false;
            mc->modFarPlane = false;
            mc->modFieldOfView = false;
            mc->modNearPlane = false;
            mc->modComponentTForm = true;
            mc->componentTform = _cameraTransform;
            mc->component = _cameraID;
            em.send(std::shared_ptr<const ModCameraComponentEvent>(mc), &_eventQueue);
        }
    }


    void PlayerControllerSystem::onTransformEntity(const TransformEntityEvent *e)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        if (_playerID && _playerID == e->entity) { // if there is a player
            _playerTransform = e->transform;
        }
    }


    void PlayerControllerSystem::onTransformEntities(const TransformEntitiesEvent *e)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        for (auto te : e->transformEvents) {
            onTransformEntity(te.get());
        }
    }


    void PlayerControllerSystem::onAddPlayerControllerComponent(const AddPlayerControllerComponentEvent *e)
    {
        if (!e) {
            DEBUG_LOG("Null event");
            return;
        }

        _playerID = e->entity;
        _cameraID = e->cameraComponent;
        _playerTransform = e->entityTform;
    }
}