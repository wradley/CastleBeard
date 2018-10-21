#include <iostream>
#include "../../Include/Graphics/System.h"
#include "../../Include/Debug.h"


Graphics::System::System()
{}


Graphics::System::~System()
{}


void Graphics::System::init(Core::EventManager & em)
{
    em.listenFor(Core::EventType::eCreateEntity, &_eventQueue);
    em.listenFor(Core::EventType::eUnloadEntities, &_eventQueue);
    em.listenFor(Core::EventType::eAddModelComponent, &_eventQueue);
    em.listenFor(Core::EventType::eAddCameraComponent, &_eventQueue);
}


void Graphics::System::update(Core::EventManager & em)
{
    handleEvents();
    _environment.draw();
}


void Graphics::System::stop(Core::EventManager & em)
{
    handleEvents();
}


void Graphics::System::handleEvents()
{
    // for loop prevents endless loop, but could still result in a lot of 
    //   iterations
    unsigned int numEvents = _eventQueue.size();
    for (unsigned int i = 0; i < numEvents; ++i) {
        auto evnt = _eventQueue.popFront();
        //std::shared_ptr<const Core::Event> evnt;
        switch (evnt->getType())
        {
        case Core::EventType::eCreateEntity:        onCreateEntityEvent((const Core::CreateEntityEvent*) evnt.get());       break;
        case Core::EventType::eUnloadEntities:      onUnloadEntitiesEvent((const Core::UnloadEntitiesEvent*) evnt.get());   break;
        case Core::EventType::eAddModelComponent:   onAddModelEvent((const Core::AddModelComponentEvent*) evnt.get());      break;
        case Core::EventType::eAddCameraComponent:  onAddCameraEvent((const Core::AddCameraComponentEvent*) evnt.get());    break; 
        default:
            DEBUG_LOG("Default case");
            break;
        }
    }
}


void Graphics::System::onCreateEntityEvent(const Core::CreateEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }
    
    // check if this entity already exists & remove children from mapping
    if (_entitiesToNodes.find(e->entity) != _entitiesToNodes.end()) {
        auto children = _environment.removeNodeAndChildren(_entitiesToNodes[e->entity]);
        for (unsigned int child : children)
            _entitiesToNodes.erase(child);
    }

    // create the new entity & mapping
    _entitiesToNodes[e->entity] = _environment.createNode();
    _environment.setNodeTransform(_entitiesToNodes[e->entity], e->transform);

    // check if entity has parent
    if (e->parent) {

        // check if we have mapping to parent
        if (_entitiesToNodes.find(e->parent) == _entitiesToNodes.end()) {
            DEBUG_LOG("Event had parent but system mapping did not");
        }

        else {
            _environment.addNodeChild(_entitiesToNodes[e->parent], _entitiesToNodes[e->entity]);
        }
    }
}


void Graphics::System::onUnloadEntitiesEvent(const Core::UnloadEntitiesEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }
    
    for (unsigned int entity : e->entities) {
        auto children = _environment.removeNodeAndChildren(_entitiesToNodes[entity]);
        _entitiesToNodes.erase(entity);
        for (unsigned int child : children)
            _entitiesToNodes.erase(child);
    }
}


void Graphics::System::onAddModelEvent(const Core::AddModelComponentEvent * e)
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
    unsigned int model = _environment.createModel();
    _environment.loadModel(model, e->filepath);
    _environment.addNodeModel(_entitiesToNodes[e->entity], model);
    _environment.setModelTransform(model, e->transform);
}


void Graphics::System::onAddCameraEvent(const Core::AddCameraComponentEvent * e)
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
    unsigned int camera = _environment.createCamera();
    _environment.addNodeCamera(_entitiesToNodes[e->entity], camera);
    _environment.setCameraTransform(camera, e->transform);
    _environment.setCameraNearPlane(camera, e->nearPlane);
    _environment.setCameraFarPlane(camera, e->farPlane);
    _environment.setCameraAspectRatio(camera, e->aspectRatio);
    _environment.setCameraFieldOfView(camera, e->fieldOfView);
    _environment.setCameraAsMain(camera);
}

