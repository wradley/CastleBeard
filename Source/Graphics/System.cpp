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
    em.listenFor(Core::EventType::eTransformEntities, &_eventQueue);
    em.listenFor(Core::EventType::eResizeWindow, &_eventQueue);
    _renderer = new Renderer;
}


void Graphics::System::update(Core::EventManager & em)
{
    handleEvents();
    _renderer->draw();
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
    case Core::EventType::eTransformEntities:
        onTransformEntities((const Core::TransformEntitiesEvent*) e.get());
        break;
    case Core::EventType::eResizeWindow:
        onResizeWindow((const Core::ResizeWindowEvent*) e.get());
        break;
    default:
    //    DEBUG_LOG("Default case");
        break;
    }
}


void Graphics::System::onCreateEntity(const Core::CreateEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // add components
    for (auto ac : e->components) {
        onEvent(ac);
    }
}


#include <assert.h>
#include "..\Physics\System.h"
void Graphics::System::onUnloadEntity(const Core::UnloadEntityEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    DEBUG_LOG("Impl. unload");
    assert(false);
}


void Graphics::System::onAddModel(const Core::AddModelComponentEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // make sure model component does not exist
    if (_componentsToModels.find(e->component) != _componentsToModels.end()) {
        DEBUG_LOG("Redefined model component");
        return;
    }

    // check if we have created an entity yet
    if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
        createSceneToEntity(e->entity, e->entityTform, e->lineage);
    }
    
    // add model to entity
    Scene *scene = _entitiesToScenes[e->entity];
    Model *model = scene->createModel();
    model->loadFile(e->filepath);
    _componentsToModels[e->component] = model;
}


void Graphics::System::onAddCamera(const Core::AddCameraComponentEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // make sure camera component does not exist
    if (_componentsToCameras.find(e->component) != _componentsToCameras.end()) {
        DEBUG_LOG("Redefined camera component");
        return;
    }

    // check if we have created an entity yet
    if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
        createSceneToEntity(e->entity, e->entityTform, e->lineage);
    }

    // add camera to entity
    Scene *scene = _entitiesToScenes[e->entity];
    Camera *camera = scene->createCamera();
    camera->setAspectRatio(e->aspectRatio);
    camera->setFarPlane(e->farPlane);
    camera->setFieldOfView(e->fieldOfView);
    camera->setLocalTransform(e->componentTform);
    camera->setNearPlane(e->nearPlane);
    _componentsToCameras[e->component] = camera;

    // TODO: better way to set main camera
    _renderer->setCamera(camera);
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

    Camera *camera = _componentsToCameras[e->component];
    if (e->modAspectRatio)    camera->setAspectRatio(e->aspectRatio);
    if (e->modComponentTForm) camera->setLocalTransform(e->componentTform);
    if (e->modFarPlane)       camera->setFarPlane(e->farPlane);
    if (e->modFieldOfView)    camera->setFieldOfView(e->fieldOfView);
    if (e->modNearPlane)      camera->setNearPlane(e->nearPlane);
}


void Graphics::System::onTransformEntity(const Core::TransformEntityEvent *e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    // check if there is proper mapping
    if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
        DEBUG_LOG("Entity [" + std::to_string(e->entity) + "] does not exist in graphics mapping");
        return;
    }

    _entitiesToScenes[e->entity]->setLocalTransform(e->transform);
}


void Graphics::System::onTransformEntities(const Core::TransformEntitiesEvent *e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    for (auto te : e->transformEvents) {
        onTransformEntity(te.get());
    }
}


void Graphics::System::onResizeWindow(const Core::ResizeWindowEvent * e)
{
    if (!e) {
        DEBUG_LOG("Given null event");
        return;
    }

    float aspect = (float)e->newWidth / (float)e->newHeight;

    for (auto pair : _componentsToCameras) {
        pair.second->setAspectRatio(aspect);
    }

    _renderer->setViewport_TEMP_((int)e->newWidth, (int)e->newHeight);
}


void Graphics::System::createSceneToEntity(
    unsigned int entity, const Math::Transform &transform,
    const std::vector<std::tuple<unsigned int, Math::Transform>> &lineage
){
    Scene *scene;

    // check if entity has lineage
    if (lineage.size() > 0) {

        // for each parent (oldest to youngest)
        for (unsigned int i = 0; i < lineage.size(); ++i) {
            auto& [pEnt, pTform] = lineage[i];

            // check if we don't already have it
            if (_entitiesToScenes.find(pEnt) == _entitiesToScenes.end()) {
                if (i == 0) { // if this is a root level scene
                    _entitiesToScenes[pEnt] = _renderer->getRootScene()->createScene();
                } else { // if this parent has another parent (not root)
                    _entitiesToScenes[pEnt] = _entitiesToScenes[std::get<0>(lineage[i - 1])]->createScene();
                }
            }

            _entitiesToScenes[pEnt]->setLocalTransform(pTform);
        }

        scene = _entitiesToScenes[std::get<0>(lineage.back())]->createScene();
    } else {
        scene = _renderer->getRootScene()->createScene();
    }
    
    // add entity to mapping
    scene->setLocalTransform(transform);
    _entitiesToScenes[entity] = scene;
}


