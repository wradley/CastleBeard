#include "../../Include/PlayerController/System.h"
#include "../../Include/Math/Quaternion.h"
#include "../../Include/Math/Matrix.h"
#include "../../Include/Math/Math.h"
#include "../../Include/Debug.h"


PlayerController::System::System() : _playerID(0)
{}


PlayerController::System::~System()
{}


void PlayerController::System::init(Core::EventManager & em)
{
    em.listenFor(Core::EventType::eCreateEntity, &_eventQueue);
    em.listenFor(Core::EventType::eUnloadEntity, &_eventQueue);
    em.listenFor(Core::EventType::eControllerInput, &_eventQueue);
    em.listenFor(Core::EventType::eTransformEntity, &_eventQueue);
    em.listenFor(Core::EventType::eAddPlayerControllerComponent, &_eventQueue);
}


void PlayerController::System::update(Core::EventManager & em)
{
    handleEvents(em);
}


void PlayerController::System::stop(Core::EventManager & em)
{
    handleEvents(em);
}


void PlayerController::System::handleEvents(Core::EventManager & em)
{
    unsigned int size = _eventQueue.size();
    for (unsigned int i = 0; i < size; ++i) {
        auto e = _eventQueue.popFront();
        switch (e->getType()) {
        case Core::EventType::eCreateEntity:
            onCreateEntity((Core::CreateEntityEvent*)e.get());
            break;
        case Core::EventType::eControllerInput:
            onControllerInput((Core::ControllerInputEvent*)e.get(), em);
            break;
        case Core::EventType::eTransformEntity:
            onTransformEntity((Core::TransformEntityEvent*)e.get());
            break;
        case Core::EventType::eAddPlayerControllerComponent:
            onAddPlayerControllerComponent((Core::AddPlayerControllerComponentEvent*)e.get());
            break;
        }
    }
}


void PlayerController::System::onCreateEntity(const Core::CreateEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Null event");
        return;
    }

    for (auto ac : e->components) {
        if (ac->getType() == Core::EventType::eAddPlayerControllerComponent) {
            onAddPlayerControllerComponent((const Core::AddPlayerControllerComponentEvent*) ac.get());
        }
    }
}


void PlayerController::System::onUnloadEntity(const Core::UnloadEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Null event");
        return;
    }

    if (e->entity == _playerID) _playerID = 0;
}


void PlayerController::System::onControllerInput(const Core::ControllerInputEvent *e, Core::EventManager &em)
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
        _playerTransform.position += dir/2.0f;

        Core::TransformEntityEvent *te = new Core::TransformEntityEvent;
        te->entity = _playerID;
        te->transform = _playerTransform;
        em.send(std::shared_ptr<const Core::TransformEntityEvent>(te), &_eventQueue);

        Core::ModCameraComponentEvent *mc = new Core::ModCameraComponentEvent;
        mc->modAspectRatio = false;
        mc->modFarPlane    = false;
        mc->modFieldOfView = false;
        mc->modNearPlane   = false;
        mc->modComponentTForm = true;
        mc->componentTform = _cameraTransform;
        mc->component = _cameraID;
        em.send(std::shared_ptr<const Core::ModCameraComponentEvent>(mc), &_eventQueue);
    }
}


void PlayerController::System::onTransformEntity(const Core::TransformEntityEvent *e)
{
    if (!e) {
        DEBUG_LOG("Null event");
        return;
    }

    if (_playerID && _playerID == e->entity) { // if there is a player
        _playerTransform = e->transform;
    }
}


void PlayerController::System::onAddPlayerControllerComponent(const Core::AddPlayerControllerComponentEvent *e)
{
    if (!e) {
        DEBUG_LOG("Null event");
        return;
    }

    _playerID = e->entity;
    _cameraID = e->cameraComponent;
    _playerTransform = e->entityTform;
}