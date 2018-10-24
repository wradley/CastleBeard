#include <iostream>
#include "../../Include/Graphics/System.h"
#include "../../Include/Debug.h"
#include "..\..\Include\PlayerController\System.h"


Graphics::System::System()
{}


Graphics::System::~System()
{}


void Graphics::System::init(Core::EventManager & em)
{
    em.listenFor(Core::EventType::eCreateEntity, &_eventQueue);
    em.listenFor(Core::EventType::eUnloadEntity, &_eventQueue);
    em.listenFor(Core::EventType::eAddModelComponent, &_eventQueue);
    em.listenFor(Core::EventType::eAddCameraComponent, &_eventQueue);
    em.listenFor(Core::EventType::eModCameraComponent, &_eventQueue);
    em.listenFor(Core::EventType::eTransformEntity, &_eventQueue);
    em.listenFor(Core::EventType::eResizeWindow, &_eventQueue);
    _environment = new Environment;
}


void Graphics::System::update(Core::EventManager & em)
{
    handleEvents();
    _environment->draw();
}


void Graphics::System::stop(Core::EventManager & em)
{
    handleEvents();
}


void Graphics::System::handleEvents()
{
    unsigned int numEvents = _eventQueue.size();
    for (unsigned int i = 0; i < numEvents; ++i) {
        onEvent(_eventQueue.popFront());
    }
}


void Graphics::System::onEvent(std::shared_ptr<const Core::Event> e)
{
    switch (e->getType())
    {
    case Core::EventType::eCreateEntity:
        onCreateEntity((const Core::CreateEntityEvent*) e.get());
        break;
    case Core::EventType::eUnloadEntity:
        onUnloadEntity((const Core::UnloadEntityEvent*) e.get());
        break;
    case Core::EventType::eAddModelComponent:
        onAddModel((const Core::AddModelComponentEvent*) e.get());
        break;
    case Core::EventType::eAddCameraComponent:
        onAddCamera((const Core::AddCameraComponentEvent*) e.get());
        break;
    case Core::EventType::eModCameraComponent:
        onModCamera((const Core::ModCameraComponentEvent*) e.get());
        break;
    case Core::EventType::eTransformEntity:
        onTransformEntity((const Core::TransformEntityEvent*) e.get());
        break;
    case Core::EventType::eResizeWindow:
        onResizeWindow((const Core::ResizeWindowEvent*) e.get());
        break;
    default:
        DEBUG_LOG("Default case");
        break;
    }
}


void Graphics::System::onCreateEntity(const Core::CreateEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }
    
    // check if this entity already exists & remove children from mapping
    if (_entitiesToNodes.find(e->entity) != _entitiesToNodes.end()) {
        auto children = _environment->removeNodeAndChildren(_entitiesToNodes[e->entity]);
        for (unsigned int child : children)
            _entitiesToNodes.erase(child);
    }

    // create the new entity & mapping
    _entitiesToNodes[e->entity] = _environment->createNode();
    _environment->setNodeTransform(_entitiesToNodes[e->entity], e->transform);

    // check if entity has parent
    if (e->parent) {

        // check if we have mapping to parent
        if (_entitiesToNodes.find(e->parent) == _entitiesToNodes.end()) {
            DEBUG_LOG("Event had parent but system mapping did not");
        }

        else {
            _environment->addNodeChild(_entitiesToNodes[e->parent], _entitiesToNodes[e->entity]);
        }
    }

    // add components
    for (auto ac : e->components) {
        onEvent(ac);
    }
}


void Graphics::System::onUnloadEntity(const Core::UnloadEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    unsigned int entity = e->entity;
    auto children = _environment->removeNodeAndChildren(_entitiesToNodes[entity]);
    _entitiesToNodes.erase(entity);
    for (unsigned int child : children)
        _entitiesToNodes.erase(child);
}


void Graphics::System::onAddModel(const Core::AddModelComponentEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // check if there is proper mapping
    if (_entitiesToNodes.find(e->entity) == _entitiesToNodes.end()) {
        DEBUG_LOG("Entity [" + std::to_string(e->entity) + "] does not exist in graphics mapping");
        return;
    }
    
    // add model to node
    unsigned int model = _environment->createModel();
    _componentsToModels[e->component] = model;
    _environment->loadModel(model, e->filepath);
    _environment->addNodeModel(_entitiesToNodes[e->entity], model);
    _environment->setModelTransform(model, e->transform);
}


void Graphics::System::onAddCamera(const Core::AddCameraComponentEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // check if there is proper mapping
    if (_entitiesToNodes.find(e->entity) == _entitiesToNodes.end()) {
        DEBUG_LOG("Entity [" + std::to_string(e->entity) + "] does not exist in graphics mapping");
        return;
    }

    // add camera to node
    unsigned int camera = _environment->createCamera();
    _componentsToCameras[e->component] = camera;
    _environment->addNodeCamera(_entitiesToNodes[e->entity], camera);
    _environment->setCameraTransform(camera, e->transform);
    _environment->setCameraNearPlane(camera, e->nearPlane);
    _environment->setCameraFarPlane(camera, e->farPlane);
    _environment->setCameraAspectRatio(camera, e->aspectRatio);
    _environment->setCameraFieldOfView(camera, e->fieldOfView);
    _environment->setCameraAsMain(camera);
}


void Graphics::System::onModCamera(const Core::ModCameraComponentEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // check if there is proper mapping
    if (_componentsToCameras.find(e->component) == _componentsToCameras.end()) {
        DEBUG_LOG("Camera Component [" + std::to_string(e->component) + "] does not exist in graphics mapping");
        return;
    }

    unsigned int camera =  _componentsToCameras[e->component];
    if (e->modAspectRatio) _environment->setCameraAspectRatio(camera, e->aspectRatio);
    if (e->modNearPlane)   _environment->setCameraNearPlane(camera, e->nearPlane);
    if (e->modFarPlane)    _environment->setCameraFarPlane(camera, e->farPlane);
    if (e->modFieldOfView) _environment->setCameraFieldOfView(camera, e->fieldOfView);
    if (e->modTransform)   _environment->setCameraTransform(camera, e->transform);
}


void Graphics::System::onTransformEntity(const Core::TransformEntityEvent *e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // check if there is proper mapping
    if (_entitiesToNodes.find(e->entity) == _entitiesToNodes.end()) {
        DEBUG_LOG("Entity [" + std::to_string(e->entity) + "] does not exist in graphics mapping");
        return;
    }

    _environment->setNodeTransform(_entitiesToNodes[e->entity], e->transform);
}


void Graphics::System::onResizeWindow(const Core::ResizeWindowEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    float aspect = (float)e->newWidth / (float)e->newHeight;

    for (auto pair : _componentsToCameras) {
        _environment->setCameraAspectRatio(pair.second, aspect);
    }

    _environment->setViewport(0, 0, e->newWidth, e->newHeight);
}
